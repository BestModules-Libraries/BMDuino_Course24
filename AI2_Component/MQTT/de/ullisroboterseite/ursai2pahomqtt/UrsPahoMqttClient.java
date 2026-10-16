package de.ullisroboterseite.ursai2pahomqtt;

// Autor: https://UllisRoboterSeite.de

// Doku:  https://UllisRoboterSeite.de/android-AI2-PahoMQTT.html
// Created: 2020-11-15
//
// Version 1.0 (2020-11-15)
// -------------------------
// - Basis-Version
//
//
// Version 1.1 (2024-09-05)
// -------------------------
// - Kategorien für Designer-Properties hinzugefügt
// - Mit neuer AI2-Version übersetzt
//
import java.util.*;

import com.google.appinventor.components.annotations.*;
import com.google.appinventor.components.common.*;
import com.google.appinventor.components.runtime.*;
import android.os.Handler;
import android.util.*;

import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import javax.net.ssl.KeyManager;
import javax.net.ssl.TrustManager;

import javax.net.SocketFactory;

@DesignerComponent(version = 1 //
        ,description = "AI2 extension block for MQTT communication." //
        ,versionName = UrsPahoMqttClient.VersionName //
        ,dateBuilt = UrsPahoMqttClient.dateBuilt //
        ,category = com.google.appinventor.components.common.ComponentCategory.EXTENSION //
        ,nonVisible = true //
        ,helpUrl = "http://UllisRoboterSeite.de/android-AI2-PahoMQTT.html" //
        ,iconName = "aiwebres/icon.png")
@SimpleObject(external = true)
@UsesPermissions(permissionNames = "android.permission.INTERNET, android.permission.ACCESS_NETWORK_STATE")
@UsesLibraries(libraries = "paho.jar")
public class UrsPahoMqttClient extends MqttBase implements MqttCallbackExtended {
    static final String LOG_TAG = "MQTT";
    static final String VersionName = "1.1.0";
    static final String dateBuilt = "2024-09-05";

    org.eclipse.paho.client.mqttv3.MqttClient pahoClient = null;

    private ArrayList<String> byteArraySubscriptionList = new ArrayList<String>();

    public UrsPahoMqttClient(ComponentContainer container) {
        super(container);
    }

    @Override
    public void connectionLost(Throwable throwable) {
        raiseIsConnectionAborted("BackgroundWorker",
                new MqttException(MqttException.REASON_CODE_CONNECTION_LOST, throwable));
        pahoClient = null;
    }

    @Override
    public void messageArrived(String topic, MqttMessage m) throws Exception {
        // Prüfen, ob Byte Array
        for (String tp : byteArraySubscriptionList) {
            if (TopicMatcher.topicMatchesSubscription(tp, topic) == TopicMatcher.MOSQ_MATCH) {
                // Ist Byte Array
                PublishedByteArrayReceived(topic, m.getPayload(), m.isRetained(), m.isDuplicate());
                return;
            }
        }

        // 'Normale'-String-Nachricht
        String msg = new String(m.getPayload());
        String result = "";
        byte[] payload = m.getPayload();
        for (int i = 0; i < payload.length; i++) {
            int temp = payload[i];
            if (temp < 0)
                temp += 256;
            result += ";" + temp;
        }
        if (result.length() > 0)
            result = result.substring(1);

        MessageReceived(topic, result, msg, m.isRetained(), m.isDuplicate());
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken t) {
        // nichts zu tun
    }

    @Override
    public void connectComplete(boolean reconnect, String serverURI) {
        raiseIsConnected("BackgroundWorker");
    }

    @SimpleFunction(description = "Connect to a MQTT broker.")
    public void Connect(boolean CleanSession) {
        final String funcName = "Connect";
        resetErrorInfo(funcName);

        if (!IsDisconnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        raiseIsConnecting(funcName);

        connect(funcName, MqttConnectHelper.buildOptions(this, CleanSession, null));
    }

    @SimpleFunction(description = "Connect to a MQTT broker.")
    public void ConnectWithLastWill(boolean CleanSession, String Topic, String Message, boolean Retain, byte QoS) {
        final String funcName = "Connect";
        resetErrorInfo(funcName);

        if (!IsDisconnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        raiseIsConnecting(funcName);

        MqttConnectOptions opts = MqttConnectHelper.buildOptions(this, CleanSession, null);
        opts.setWill(Topic, Message.getBytes(), QoS, Retain);

        connect(funcName, opts);
    }

    void connect(final String funcName, final MqttConnectOptions connOpts) {
        // neuen Client erstellen
        MemoryPersistence persistence = new MemoryPersistence();

        if (connOpts.isCleanSession())
            byteArraySubscriptionList = new ArrayList<String>();

        if (clientID.isEmpty())
            clientID = UUID.randomUUID().toString();

        String serverUri = "";

        if (protocol.equals("TCP")) {
            serverUri = "tcp://" + broker + ":" + port;
            Log.d(LOG_TAG, "Protokoll tcp");
        } else {
            serverUri = "ssl://" + broker + ":" + port;
            Log.d(LOG_TAG, "Protokoll ssl");

            SocketFactory sf;

            TrustManager[] tm = SslHelper.CreateTMs();
            KeyManager[] km = SslHelper.CreateKMs();

            if (!trustedCertFile.isEmpty()) { // Prüfung gegen Zertifikat-Datei
                try {
                    Log.d(LOG_TAG, "Trusted certificate file");
                    tm = SslHelper.CreateTMs(form, trustedCertFile);
                } catch (Exception e) {
                    Log.d(LOG_TAG, "Fehler CreateTMs (trustedCertFile):" + e.toString());
                    raiseIsConnectionAborted(funcName,
                            new MqttException(Helper.REASON_CODE_INVALID_TRUSTEDCERT, e));
                }

            } else if (!truststoreFile.isEmpty()) { // Prüfung gegen Truststore
                try {
                    Log.d(LOG_TAG, "Truststore");
                    tm = SslHelper.CreateTMs(form, truststoreFile, truststorePassword);
                } catch (Exception e) {
                    Log.d(LOG_TAG, "Fehler CreateTMs (trustStoreFile):" + e.toString());
                    raiseIsConnectionAborted(funcName,
                            new MqttException(Helper.REASON_CODE_INVALID_TRUSTSTORE, e));
                }
            } else {
                Log.d(LOG_TAG, "CA certificate");
                tm = SslHelper.CreateTMs(); // keine Exception
            }

            if (!clientCertFile.isEmpty()) {
                try {
                    Log.d(LOG_TAG, "Client certificate file");
                    km = SslHelper.CreateKMs(form, clientCertFile, clientKeyFile, clientKeyPassword,
                            clientPemFormatted);
                } catch (Exception e) {
                    Log.d(LOG_TAG, "Fehler CreateKMs (clientCertFile):" + e.toString());
                    raiseIsConnectionAborted(funcName,
                            new MqttException(Helper.REASON_CODE_INVALID_CLIENTCERT, e));
                }

            } else if (!clientKeystoreFile.isEmpty()) {
                try {
                    Log.d(LOG_TAG, "Client keystore file");
                    km = SslHelper.CreateKMs(form, clientKeystoreFile, clientKeystorePassword, clientKeyPassword);
                } catch (Exception e) {
                    Log.d(LOG_TAG, "Fehler CreateKMs (clientKeyStoreFile):" + e.toString());
                    raiseIsConnectionAborted(funcName,
                            new MqttException(Helper.REASON_CODE_INVALID_CLIENTKEYSTORE, e));
                }
            } else {
                Log.d(LOG_TAG, "No client certifaction");
                km = SslHelper.CreateKMs(); // keine Exception
            }

            try {
                sf = SslHelper.createSSLSocketFactory(tm, km, protocol);
                connOpts.setSocketFactory(sf);
            } catch (Exception e) {
                Log.d(LOG_TAG, "Fehler SocketFactory:" + e.toString());
                raiseIsConnectionAborted(funcName, new MqttException(MqttException.REASON_CODE_SSL_CONFIG_ERROR, e));
                return;
            }
        }

        try {
            pahoClient = new MqttClient(serverUri, clientID, persistence);
        } catch (IllegalArgumentException ex) {
            raiseIsConnectionAborted(funcName, new MqttException(Helper.REASON_CODE_INVALID_CON_PARAMETERS, ex));
            return;
        } catch (MqttException me) {
            raiseIsConnectionAborted(funcName, me);
            return;
        } catch (Exception e) {
            raiseIsConnectionAborted(funcName, new MqttException(MqttException.REASON_CODE_UNEXPECTED_ERROR, e));
            return;
        }

        long t = -1;
        if (timeToWait >= 0)
            t = timeToWait * 1000;
        pahoClient.setTimeToWait(t);

        pahoClient.setCallback(this);

        // Connect in separatem Thread ausführen
        Helper.Runner.start(new Runnable() {
            public void run() {
                try {
                    Log.d(LOG_TAG, "Thread connect");
                    pahoClient.connect(connOpts);
                } catch (MqttException me) {
                    Log.d(LOG_TAG, "Thread connect err: " + me.toString());
                    raiseIsConnectionAborted(funcName, me);
                }
            } // run
        }); // Runnable);
    }

    @SimpleFunction(description = "Subscribe a topic.")
    public void Subscribe(String Topic, byte QoS) {
        final String funcName = "Subscribe";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        try {
            pahoClient.subscribe(Topic, QoS);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
        }
    }

    @SimpleFunction(description = "Subscribe a topic for receiving byte arrays.")
    public void SubscribeByteArray(String Topic, byte QoS) {
        final String funcName = "SubscribeByteArray";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        try {
            pahoClient.subscribe(Topic, QoS);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }

        if (!byteArraySubscriptionList.contains(Topic))
            byteArraySubscriptionList.add(Topic);
    }

    @SimpleFunction(description = "Unsubscribe a topic.")
    public void Unsubscribe(String Topic) {
        final String funcName = "Unsubscribe";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        byteArraySubscriptionList.remove(Topic);

        try {
            pahoClient.unsubscribe(Topic);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }
    }

    @SimpleFunction(description = "Publish a MQTT message.")
    public void PublishEx(String Topic, String Message, boolean RetainFlag, int QoS) {
        final String funcName = "PublishEx";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        try {
            pahoClient.publish(Topic, Message.getBytes(), QoS, RetainFlag);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }

    }

    @SimpleFunction(description = "Publish a MQTT message. Retain flag is false, QoS is 0.")
    public void Publish(String Topic, String Message) {
        final String funcName = "PublishEx";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        try {
            pahoClient.publish(Topic, Message.getBytes(), 0, false);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }
    }

    byte[] StringToBytes(String inp) throws NumberFormatException {
        inp = inp.replace(',', ';');
        String[] bs = inp.split(";");
        byte[] bytes = new byte[bs.length];

        for (int i = 0; i < bs.length; i++) {
            int b = Integer.decode(bs[i].trim());
            if (b > 255 || b < 0)
                throw new NumberFormatException();
            bytes[i] = (byte) b;
        }
        return bytes;
    }

    @SimpleFunction(description = "Publishes a binary coded MQTT message.")
    public void PublishBinary(String Topic, String BinaryMessage, boolean RetainFlag, int QoS) {
        final String funcName = "PublishBinary";
        byte[] bytes;

        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        try {
            bytes = StringToBytes(BinaryMessage);
        } catch (Exception e) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_CONVERT_ERROR));
            return;
        }

        try {
            pahoClient.publish(Topic, bytes, QoS, RetainFlag);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }
    }

    @SimpleFunction(description = "Publishes a binary array.")
    public void PublishByteArray(String Topic, Object ByteArray, boolean RetainFlag, int QoS) {
        final String funcName = "PublishByteArray";

        resetErrorInfo(funcName);

        if (!IsConnected()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_INVALID_STATE));
            return;
        }

        if (Topic.isEmpty()) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_EMPTY_TOPIC));
            return;
        }

        if (ByteArray == null) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_NOT_BYTE_ARRAY));
            return;
        }

        if (!ByteArray.getClass().equals(byte[].class)) {
            raiseErrorOccured(funcName, new MqttException(Helper.REASON_CODE_NOT_BYTE_ARRAY));
            return;
        }

        try {
            pahoClient.publish(Topic, (byte[]) ByteArray, QoS, RetainFlag);
        } catch (MqttException me) {
            raiseErrorOccured(funcName, me);
            return;
        }
    }

    @SimpleFunction(description = "Test whether an object is null.")
    public boolean IsNull(Object Object) {
        return Object == null;
    }

    @SimpleFunction(description = "Disconnects from the broker.")
    public void Disconnect() {
        final String funcName = "Disconnect";
        resetErrorInfo(funcName);

        if (!IsConnected()) {
            form.ErrorOccurred(this, funcName, -1, "Invalid State");
            return;
        }

        raiseIsDisconnecting();

        try {
            pahoClient.disconnect();
        } catch (MqttException me) {
            // Nichts zu tun
        }
        pahoClient = null;
        raiseIsDisconnected();
    }
}