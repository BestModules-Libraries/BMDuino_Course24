package de.ullisroboterseite.ursai2pahomqtt;

import java.security.KeyFactory;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Security;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.SSLSocketFactory;

import com.google.appinventor.components.runtime.Form;
import android.util.Log;

import org.eclipse.paho.client.mqttv3.*;

class SslHelper {
    static final String LOG_TAG = UrsPahoMqttClient.LOG_TAG;

    static SSLSocketFactory createSSLSocketFactory(TrustManager[] tm, KeyManager[] km, String tlsVersion)
            throws Exception {
        try {
            SSLContext context = SSLContext.getInstance(tlsVersion);
            context.init(km, tm, null);
            return context.getSocketFactory();
        } catch (Exception e) {
            throw new MqttException(MqttException.REASON_CODE_SSL_CONFIG_ERROR, e);
        }
    }

    // Leerer Trustmanager-Liste
    static TrustManager[] CreateTMs() {
        Log.d(LOG_TAG, "CreateTMs server CAcertificate");
        return null;
    }

    // Zertifikat-Datei
    static TrustManager[] CreateTMs(Form form, String trustedCertFile) throws Exception {
        Log.d(LOG_TAG, "CreateTMs, trustedCertFile: <" + trustedCertFile + ">");
        X509Certificate caCert = SslLoader.loadX509Certificate(form, trustedCertFile);
        KeyStore caKs = KeyStore.getInstance(KeyStore.getDefaultType());
        caKs.load(null, null);
        caKs.setCertificateEntry("ca-certificate", caCert);
        TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        tmf.init(caKs);

        return tmf.getTrustManagers();
    }

    // Keystore
    static TrustManager[] CreateTMs(Form form, String trustStoreFile, String trustedPassword) throws Exception {
        Log.d(LOG_TAG, "CreateTMs, trustStoreFile: <" + trustStoreFile + ">");
        KeyStore caKs = SslLoader.loadKeystore(form, trustStoreFile, trustedPassword);
        TrustManagerFactory tmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        tmf.init(caKs);

        return tmf.getTrustManagers();
    }

    static KeyManager[] CreateKMs() {
        Log.d(LOG_TAG, "CreateKMs no client certification");
        return null;
    }

    static KeyManager[] CreateKMs(Form form, String clientCertFile, String privateKeyFile, String password,
            boolean pemFormat) throws Exception {
        Log.d(LOG_TAG, "CreateKMs, clientCertFile: <" + clientCertFile + ">");
        KeyStore ks = KeyStore.getInstance(KeyStore.getDefaultType());
        ks.load(null, null);

        X509Certificate clientCert = pemFormat ? SslLoader.loadX509CertificatePem(form, clientCertFile)
                : SslLoader.loadX509Certificate(form, clientCertFile);
        ks.setCertificateEntry("certificate", clientCert);

        PrivateKey privateKey = pemFormat ? SslLoader.loadPrivateKeyPem(form, privateKeyFile, "RSA")
                : SslLoader.loadPrivateKeyHex(form, privateKeyFile, "RSA");
        ks.setKeyEntry("private-key", privateKey, password.toCharArray(), new Certificate[] { clientCert });

        KeyManagerFactory kmf = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
        kmf.init(ks, password.toCharArray());

        return kmf.getKeyManagers();
    }

    static KeyManager[] CreateKMs(Form form, String clientKeystoreFile, String clientKeystorePassword,
            String clientKeyPairPassword) throws Exception {
        Log.d(LOG_TAG, "CreateKMs, clientKeystoreFile: <" + clientKeystoreFile + ">");
        KeyStore ks = SslLoader.loadKeystore(form, clientKeystoreFile, clientKeystorePassword);
        KeyManagerFactory kmf = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
        kmf.init(ks, clientKeyPairPassword.toCharArray());

        return kmf.getKeyManagers();
    }
}