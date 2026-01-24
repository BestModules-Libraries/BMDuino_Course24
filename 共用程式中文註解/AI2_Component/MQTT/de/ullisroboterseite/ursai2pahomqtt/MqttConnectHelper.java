package de.ullisroboterseite.ursai2pahomqtt;

import com.google.appinventor.components.annotations.*;
import com.google.appinventor.components.common.*;
import com.google.appinventor.components.runtime.*;

import android.os.Handler;
import android.util.*;

import java.util.*;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.*;
import java.security.cert.*;

import javax.net.SocketFactory;
import javax.net.ssl.*;

import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;

// Datei C:\Program Files\mqttfx
// package de.jensd.mqttfx.util / de.jensd.mqttfx.ssl;
// Klasse MqttConnectOptionsFactory / SSLFellow
// Methode createMqttConnectOptions / createSSLSocketFactory

public class MqttConnectHelper {
    static final String LOG_TAG = UrsPahoMqttClient.LOG_TAG;

    static MqttConnectOptions buildOptions(MqttBase props, boolean cleanSession, SocketFactory sf) {
        MqttConnectOptions connOpts = new MqttConnectOptions();
        connOpts.setCleanSession(cleanSession);
        connOpts.setConnectionTimeout(props.connectionTimeout);
        connOpts.setKeepAliveInterval(props.keepAlive);
        connOpts.setSocketFactory(sf);

        if (!props.userName.isEmpty())
            connOpts.setUserName(props.userName);
        if (!props.userPassword.isEmpty())
            connOpts.setPassword(props.userPassword.toCharArray());

        return connOpts;
    }


}