package de.ullisroboterseite.ursai2pahomqtt;

import java.util.*;

import com.google.appinventor.components.annotations.*;
import com.google.appinventor.components.common.*;
import com.google.appinventor.components.runtime.*;
import com.google.appinventor.components.runtime.util.*;
import android.os.Handler;

import android.util.*;

import org.eclipse.paho.client.mqttv3.*;

// Kapselt Eigenschaften und Ereignisse

@SimpleObject(external = true)
@UsesPermissions(permissionNames = "android.permission.INTERNET, android.permission.ACCESS_NETWORK_STATE")
public class MqttBase extends AndroidNonvisibleComponent {
    static final String LOG_TAG = "MQTT";
    final Handler handler = new Handler();

    Form form;
    final Component thisComponent = this;

    String broker = "";
    int port = 1883;
    int connectionTimeout = 30;
    int timeToWait = -1;
    int keepAlive = 60;
    String clientID = "";
    String userName = "";
    String userPassword = "";
    String protocol = "TCP";
    int maxInflight = 10;

    String trustedCertFile = "";
    String truststoreFile = "";
    String truststorePassword = "";

    String clientCertFile = "";
    String clientKeyFile = "";
    String clientKeyPassword = "";
    boolean clientPemFormatted = false;

    String clientKeystoreFile = "";
    String clientKeystorePassword = "";

    String lastErrMsg = ""; // Text des letzten Fehlers
    int lastErrorCode = 0; // Fehlercode des letzten Fehlers
    String lastAction = ""; // Fehlercode des letzten Fehlers
    String exceptionCause = ""; // ggf. die letzte Exception

    MqttConnectionState connectionState = MqttConnectionState.Disconnected;

    public MqttBase(ComponentContainer container) {
        super(container.$form());
        this.form = container.$form();
    } // ctor

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "The protocol to use.")
    public String Protocol() {
        return protocol;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_CHOICES, //
            editorArgs = { "TCP", "SSL", "TLS" }, defaultValue = "TCP")
    @SimpleProperty(description = "The protocol to use.")
    public void Protocol(String value) {
        value = value.toUpperCase().trim();
        if (value.equals("TCP") || value.equals("SSL") || value.equals("TLS"))
            protocol = value;
        else
            raiseErrorOccured("Set Protocol", new MqttException(MqttException.REASON_CODE_INVALID_PROTOCOL_VERSION));
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The IP address or hostname of the server to connect to.")
    public String Broker() {
        return broker;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The IP address or hostname of the server to connect to.")
    public void Broker(String value) {
        broker = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The port number of the server to connect to.")
    public int Port() {
        return port;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_NON_NEGATIVE_INTEGER, defaultValue = "1883")
    @SimpleProperty(description = "The port number of the server to connect to.")
    public void Port(int value) {
        if (value > 0)
            port = value;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Connection timeout [seconds].")
    public int ConnectionTimeout() {
        return connectionTimeout;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_NON_NEGATIVE_INTEGER, defaultValue = "30")
    @SimpleProperty(description = "Connection timeout [seconds].")
    public void ConnectionTimeout(int value) {
        if (value >= 0)
            connectionTimeout = value;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Keep alive interval [seconds].")
    public int KeepAlive() {
        return keepAlive;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_NON_NEGATIVE_INTEGER, defaultValue = "60")
    @SimpleProperty(description = "Keep alive interval [seconds].")
    public void KeepAlive(int value) {
        if (value > 0)
            keepAlive = value;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "Maximum time to wait for an action to complete [seconds].\n"
            + "-1 means the action will not timeout.")
    public int TimeToWait() {
        return timeToWait;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_INTEGER, defaultValue = "-1")
    @SimpleProperty(description = "Maximum time to wait for an action to complete [seconds].\n"
            + "-1 means the action will not timeout.")
    public void TimeToWait(int value) {
        timeToWait = value;
    }

    @SimpleProperty(category = PropertyCategory.BEHAVIOR, description = "The max inflight limits to how many messages we can send without receiving acknowledgments.\n"
            + "Increase this value in a high traffic environment.")
    public int MaxInflight() {
        return maxInflight;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_NON_NEGATIVE_INTEGER, defaultValue = "10")
    @SimpleProperty(description = "The max inflight limits to how many messages we can send without receiving acknowledgments.\n"
            + "Increase this value in a high traffic environment.")
    public void MaxInflight(int value) {
        maxInflight = value;
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The unique client Id. If this field is blank a random GUID is used.")
    public String ClientID() {
        return clientID;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The unique client Id. If this field is blank, a random GUID is used.")
    public void ClientID(String value) {
        clientID = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The user name used authentication and authorization.")
    public String UserName() {
        return userName;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The user name used authentication and authorization.")
    public void UserName(String value) {
        userName = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The password used authentication and authorization.")
    public String UserPassword() {
        return userPassword;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The password used authentication and authorization.")
    public void UserPassword(String value) {
        userPassword = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The name of the trusted certificate file.")
    public String TrustedCertFile() {
        return trustedCertFile;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_ASSET, defaultValue = "")
    @SimpleProperty(description = "The name of the trusted certificate file.")
    public void TrustedCertFile(String value) {
        trustedCertFile = value.trim();
        Log.d(LOG_TAG, "Set TrustedCertFile: <" + TrustedCertFile() + ">");
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The name of the truststore file.")
    public String TruststoreFile() {
        return truststoreFile;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_ASSET, defaultValue = "")
    @SimpleProperty(description = "The name of the truststore file.")
    public void TruststoreFile(String value) {
        truststoreFile = value.trim();
        Log.d(LOG_TAG, "Set TruststoreFile: <" + TruststoreFile() + ">");
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The password of the truststore file.")
    public String TruststorePassword() {
        return truststorePassword;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_TEXT, defaultValue = "")
    @SimpleProperty(description = "The password of the truststore file.")
    public void TruststorePassword(String value) {
        truststorePassword = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The name of the client certificate file.")
    public String ClientCertFile() {
        return clientCertFile;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_ASSET, defaultValue = "")
    @SimpleProperty(description = "The name of the client certificate file.")
    public void ClientCertFile(String value) {
        clientCertFile = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The name of the client key file.")
    public String ClientKeyFile() {
        return clientKeyFile;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_ASSET, defaultValue = "")
    @SimpleProperty(description = "The name of the client key file.")
    public void ClientKeyFile(String value) {
        clientKeyFile = value.trim();
        Log.d(LOG_TAG, "Set ClientKeyFile: <" + ClientKeyFile() + ">");
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The client key password.")
    public String ClientKeyPassword() {
        return clientKeyPassword;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The client key password.")
    public void ClientKeyPassword(String value) {
        clientKeyPassword = value.trim();
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The client certifacte and key files are PEM formatted.")
    public boolean ClientPemFormatted() {
        return clientPemFormatted;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_BOOLEAN, defaultValue = "")
    @SimpleProperty(description = "The client certifacte and key files are PEM formatted.")
    public void ClientPemFormatted(boolean value) {
        clientPemFormatted = value;
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The name of the client keystore file.")
    public String ClientKeystoreFile() {
        return clientKeystoreFile;

    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_ASSET, defaultValue = "")
    @SimpleProperty(description = "The name of the client keystore file.")
    public void ClientKeystoreFile(String value) {
        clientKeystoreFile = value.trim();
        Log.d(LOG_TAG, "Set ClientKeystoreFile: <" + ClientKeystoreFile() + ">");
    }

    @SimpleProperty(category = PropertyCategory.GENERAL, description = "The client keystore password.")
    public String ClientKeystorePassword() {
        return clientKeystorePassword;
    }

    @DesignerProperty(editorType = PropertyTypeConstants.PROPERTY_TYPE_STRING, defaultValue = "")
    @SimpleProperty(description = "The client keystore password.")
    public void ClientKeystorePassword(String value) {
        clientKeystorePassword = value.trim();
    }

    @SimpleProperty(description = "true: Client is connected to a MQTT broker.")
    public boolean IsConnected() {
        return connectionState == MqttConnectionState.Connected;
    }

    @SimpleProperty(description = "true: Client is disconnected from the MQTT broker.")
    public boolean IsDisconnected() {
        return (connectionState == MqttConnectionState.Disconnected)
                || (connectionState == MqttConnectionState.ConnectionAbortet);
    }

    @SimpleProperty(description = "The connection state:\n" //
            + "0: Disconnected. The client is not connected to a broker.\n" //
            + "1: Connecting. The client is currently creating a connection to a MQTT broker.\n" //
            + "2: Connected. The client is connected to a MQTT broker.\n" //
            + "3: Disconnecting. The client is currently disconnecting from the MQTT broker.\n" //
            + "4: ConnectionAbortet. The connection could not be established or was interrupted.")
    public int ConnectionState() {
        return connectionState.ordinal();
    }

    @SimpleProperty(description = "Constant for connection state 'Disconnected'.")
    public int StateDisconnected() {
        return MqttConnectionState.Disconnected.ordinal();
    }
    @SimpleProperty(description = "Constant for connection state 'Connecting'.")
    public int StateConnecting() {
        return MqttConnectionState.Connecting.ordinal();
    }
    @SimpleProperty(description = "Constant for connection state 'Connected'.")
    public int StateConnected() {
        return MqttConnectionState.Connected.ordinal();
    }
    @SimpleProperty(description = "Constant for connection state 'Disconnecting'.")
    public int StateDisconnecting() {
        return MqttConnectionState.Disconnecting.ordinal();
    }
    @SimpleProperty(description = "Constant for connection state 'ConnectionAbortet'.")
    public int StateConnectionAbortet() {
        return MqttConnectionState.ConnectionAbortet.ordinal();
    }

    @SimpleEvent(description = "Connection state has changed.")
    public void ConnectionStateChanged(int NewState, String StateString) {
        EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", NewState, StateString);
    }

    // Ereignis ConnectionStateChanged wird sofort ausgelöst
    void raiseIsConnecting(String ActionName) {
        connectionState = MqttConnectionState.Connecting;
        EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", connectionState.ordinal(),
                connectionState.toString());
    }

    // Ereignis ConnectionStateChanged wird verspätet ausgelöst
    void raiseIsConnected(String ActionName) {
        connectionState = MqttConnectionState.Connected;
        final int newState = connectionState.ordinal();
        final String stateString = connectionState.toString();
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", newState, stateString);
            }
        });
    }

    // Ereignis ConnectionStateChanged wird verspätet ausgelöst
    void raiseIsConnectionAborted(String functionName, MqttException me) {
        connectionState = MqttConnectionState.ConnectionAbortet;

        lastAction = functionName;
        lastErrMsg = Helper.mqttException2Message(me);
        lastErrorCode = me.getReasonCode();

        exceptionCause = "";
        Throwable cause = me.getCause();
        if (cause != null)
            exceptionCause = cause.toString();

        final int newState = connectionState.ordinal();
        final String stateString = connectionState.toString();
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", newState, stateString);
            }
        });
    }

    // Ereignis ConnectionStateChanged wird sofort ausgelöst
    void raiseIsDisconnecting() {
        connectionState = MqttConnectionState.Disconnecting;
        EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", connectionState.ordinal(),
                connectionState.toString());
    }

    // Ereignis ConnectionStateChanged wird verspätet ausgelöst
    void raiseIsDisconnected() {
        connectionState = MqttConnectionState.Disconnected;
        final int newState = connectionState.ordinal();
        final String stateString = connectionState.toString();
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "ConnectionStateChanged", newState, stateString);
            }
        });
    }

    @SimpleEvent(description = "Message received.")
    public void MessageReceived(final String Topic, final String Payload, final String Message,
            final boolean RetainFlag, final boolean DupFlag) {
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "MessageReceived", Topic, Payload, Message, RetainFlag,
                        DupFlag);
            }
        });
    }

    @SimpleEvent(description = "Message with byte array received.")
    public void PublishedByteArrayReceived(final String Topic, final Object ByteArray, final boolean RetainFlag,
            final boolean DupFlag) {
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "PublishedByteArrayReceived", Topic, ByteArray, RetainFlag,
                        DupFlag);
            }
        });
    }

    @SimpleProperty(description = "Returns a text message about the last error.")
    public String LastErrorMessage() {
        return lastErrMsg;
    }

    @SimpleProperty(description = "Returns the code of the last error.")
    public int LastErrorCode() {
        return lastErrorCode;
    }

    @SimpleProperty(description = "Returns the last Action the error code belongs to.")
    public String LastAction() {
        return lastAction;
    }

    @SimpleProperty(description = "Provides information on the last exception.")
    public String LastExecptionCause() {
        return exceptionCause;
    }

    void resetErrorInfo(String ActionName) {
        lastErrMsg = ""; // Text des letzten Fehlers
        lastErrorCode = 0; // Fehlercode des letzten Fehlers
        ActionName = ""; // Fehlercode des letzten Fehlers
        exceptionCause = ""; // ggf. die letzte Exception
    }

    @SimpleEvent(description = "Error occurred.")
    public void ErrorOccurred(final String ActionName, final int ErrorCode, final String ErrorMessage) {
        handler.post(new Runnable() {
            public void run() {
                EventDispatcher.dispatchEvent(thisComponent, "ErrorOccurred", ActionName, ErrorCode, ErrorMessage);
            }
        });
    }

    void raiseErrorOccured(String ActionName, MqttException me) {
        exceptionCause = "";
        Throwable cause = me.getCause();
        if (cause != null)
            exceptionCause = cause.toString();

        lastErrMsg = Helper.mqttException2Message(me);
        lastErrorCode = me.getReasonCode();
        lastAction = ActionName;
        ErrorOccurred(ActionName, lastErrorCode, lastErrMsg);
    }

    @SimpleFunction(description = "Export conection parameters to a dictionary.")
    public YailDictionary ToDictionary() {
        YailDictionary dict = new YailDictionary();
        dict.put("Broker", broker);
        dict.put("Port", port);
        dict.put("ConnectionTimeout", connectionTimeout);
        dict.put("TimeToWait", timeToWait);
        dict.put("KeepAlive", keepAlive);
        dict.put("ClientID", clientID);
        dict.put("UserName", userName);
        dict.put("UserPassword", userPassword);
        dict.put("Protocol", protocol);
        dict.put("MaxInflight", maxInflight);

        dict.put("TrustedCertFile", trustedCertFile);
        dict.put("TruststoreFile", truststoreFile);
        dict.put("TruststorePassword", truststorePassword);

        dict.put("ClientCertFile", clientCertFile);
        dict.put("ClientKeyFile", clientKeyFile);
        dict.put("ClientKeyPassword", clientKeyPassword);
        dict.put("ClientPemFormatted", clientPemFormatted);
        dict.put("ClientKeystoreFile", clientKeystoreFile);
        dict.put("ClientKeystorePassword", clientKeystorePassword);
        return dict;
    }

    int getInt(YailDictionary dict, String key) throws Exception {
        Object o = dict.get(key);
        if (o == null)
            throw new Exception();
        return (int) o;
    }

    String getString(YailDictionary dict, String key) throws Exception {
        Object o = dict.get(key);
        if (o == null)
            throw new Exception();
        return (String) o;
    }

    boolean getBoolean(YailDictionary dict, String key) throws Exception {
        Object o = dict.get(key);
        if (o == null)
            throw new Exception();
        return (boolean) o;
    }

    @SimpleFunction(description = "Export conection parameters to a dictionary.")
    public void FromDictionary(YailDictionary dict) {
        try {
            getString(dict, "Broker");
            getInt(dict, "Port");
            getInt(dict, "ConnectionTimeout");
            getInt(dict, "TimeToWait");
            getInt(dict, "KeepAlive");
            getString(dict, "ClientID");
            getString(dict, "UserName");
            getString(dict, "UserPassword");
            getString(dict, "Protocol");
            getInt(dict, "MaxInflight");
            getString(dict, "TrustedCertFile");
            getString(dict, "TruststoreFile");
            getString(dict, "TruststorePassword");
            getString(dict, "ClientCertFile");
            getString(dict, "ClientKeyFile");
            getString(dict, "ClientKeyPassword");
            getBoolean(dict, "ClientPemFormatted");

            getString(dict, "ClientKeystoreFile");
            getString(dict, "ClientKeystorePassword");

        } catch (Exception e) {
            raiseErrorOccured("FromDictionary", new MqttException(Helper.REASON_CODE_INVALID_DICTIONARY));
            return;
        }
        // Ok, alle notwendigen Einträge vorhanden
        try {
            broker = getString(dict, "Broker");
            port = getInt(dict, "Port");
            connectionTimeout = getInt(dict, "ConnectionTimeout");
            timeToWait = getInt(dict, "TimeToWait");
            keepAlive = getInt(dict, "KeepAlive");
            clientID = getString(dict, "ClientID");
            userName = getString(dict, "UserName");
            userPassword = getString(dict, "UserPassword");
            protocol = getString(dict, "Protocol");
            maxInflight = getInt(dict, "MaxInflight");

            trustedCertFile = getString(dict, "TrustedCertFile");
            truststoreFile = getString(dict, "TruststoreFile");
            truststorePassword = getString(dict, "TruststorePassword");

            clientCertFile = getString(dict, "ClientCertFile");
            clientKeyFile = getString(dict, "ClientKeyFile");
            clientKeyPassword = getString(dict, "ClientKeyPassword");
            clientPemFormatted = getBoolean(dict, "ClientPemFormatted");

            clientKeystoreFile = getString(dict, "ClientKeystoreFile");
            clientKeystorePassword = getString(dict, "ClientKeystorePassword");
        } catch (Exception e) {
            //nichts zu tun
        }
    }
}