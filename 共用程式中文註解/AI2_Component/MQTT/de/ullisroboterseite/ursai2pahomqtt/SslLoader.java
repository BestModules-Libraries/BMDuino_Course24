package de.ullisroboterseite.ursai2pahomqtt;

import android.util.Base64;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.List;

import com.google.appinventor.components.runtime.Form;

public class SslLoader {
    static final String LOG_TAG = UrsPahoMqttClient.LOG_TAG;

    // Einlesen eines Zertifikats aus einer Asset-Datei
    static X509Certificate loadX509Certificate(Form form, String crtFile) throws Exception {
        Log.d(LOG_TAG, "loadX509Certificate, crtFile: <" + crtFile + ">");
        CertificateFactory cf = CertificateFactory.getInstance("X.509");
        InputStream inStream = null;

        inStream = form.openAsset(crtFile);

        X509Certificate certificate = (X509Certificate) cf.generateCertificate(inStream);
        inStream.close();
        return certificate;
    }

    // Einlesen eines Zertifikats im PEM-Format aus einer Asset-Datei
    static X509Certificate loadX509CertificatePem(Form form, String crtFile) throws Exception {
        Log.d(LOG_TAG, "loadX509CertificatePem, crtFile: <" + crtFile + ">");
        CertificateFactory cf = CertificateFactory.getInstance("X.509");
        InputStream inStream = form.openAsset(crtFile);

        X509Certificate certificate = (X509Certificate) cf.generateCertificate(inStream);

        return certificate;
    }

    // Einlesen eines Keystore aus einer Asset-Datei
    public static KeyStore loadKeystore(Form form, String keyStoreFile, String password) throws Exception {
        Log.d(LOG_TAG, "LoadKeystore, keyStoreFile: <" + keyStoreFile + ">");
        InputStream inStream = form.openAsset(keyStoreFile);
        KeyStore keyStore;
        if (keyStoreFile.toLowerCase().endsWith(".p12") || keyStoreFile.toLowerCase().endsWith(".pfx")) {
            Log.d(LOG_TAG, "LoadKeystore PKS12");
            keyStore = KeyStore.getInstance("pkcs12");
        }
        else
            keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
        keyStore.load(inStream, password.toCharArray());
        return keyStore;
    }

    // Einlesen eines Private Key aus einer Asset-Datei
    static PrivateKey loadPrivateKeyHex(Form form, String keyFile, String algorithm) throws Exception {
        return KeyFactory.getInstance(algorithm).generatePrivate(new PKCS8EncodedKeySpec(loadHex(form, keyFile)));
    }

    // Einlesen eines Private Key im PEM-Format aus einer Asset-Datei
    static PrivateKey loadPrivateKeyPem(Form form, String keyPemFile, String algorithm) throws Exception {
        return KeyFactory.getInstance(algorithm)
                .generatePrivate(new PKCS8EncodedKeySpec(loadPemContent(form, keyPemFile)));
    }

    // Einlesen eines Public Key aus einer Asset-Datei
    static PublicKey loadPublicKeyHex(Form form, String keyFile, String algorithm) throws Exception {
        return KeyFactory.getInstance(algorithm).generatePublic(new X509EncodedKeySpec(loadHex(form, keyFile)));
    }

    // Einlesen eines Public Key im PEM-ormat aus einer Asset-Datei
    static PublicKey loadPublicKeyPem(Form form, String keyPemFile, String algorithm) throws Exception {
        return KeyFactory.getInstance(algorithm)
                .generatePublic(new X509EncodedKeySpec(loadPemContent(form, keyPemFile)));
    }

    // Einlesen eines Schlüssespaares aus Dateien
    static KeyPair loadKeyPair(Form form, String publicKeyFile, String privateKeyFile, String algorithm)
            throws Exception {
        return new KeyPair(loadPublicKeyHex(form, publicKeyFile, algorithm),
                loadPrivateKeyHex(form, privateKeyFile, algorithm));
    }

    // Einlesen eines Schlüssespaares im PEM-Format aus Dateien
    static KeyPair loadKeyPairPem(Form form, String publicKeyPemFile, String privateKeyPemFile, String algorithm)
            throws Exception {
        return new KeyPair(loadPublicKeyPem(form, publicKeyPemFile, algorithm),
                loadPrivateKeyPem(form, privateKeyPemFile, algorithm));
    }

    // Einlesen eines Schlüssespaares aus einem Keystore in einer Asset-Datei
    /*
    static KeyPair loadKeyPairFromKeystore(Form form, String keyStoreFile, String keyStorePassword, String alias,
            String aliasPassword) throws Exception {
        KeyStore keyStore = loadKeystore(form, keyStoreFile, keyStorePassword);
        Key key = keyStore.getKey(alias, aliasPassword.toCharArray());
        if (key instanceof PrivateKey) {
            return new KeyPair(keyStore.getCertificate(alias).getPublicKey(), (PrivateKey) key);
        }
        return null;
    }
    */
    static byte[] loadHex(Form form, String file) throws Exception {
        Log.d(LOG_TAG, "load hex");
        InputStream inStream = form.openAsset(file);
        byte[] encodedData = new byte[inStream.available()];
        inStream.read(encodedData);
        inStream.close();
        return encodedData;
    }

    private static final String BEGIN = "-----BEGIN ";
    private static final String END = "-----END ";

    static byte[] loadPemContent(Form form, String file) throws Exception {
        Log.d(LOG_TAG, "load pem");
        InputStream inStream = form.openAsset(file);
        BufferedReader reader = new BufferedReader(new InputStreamReader(inStream));

        String line = reader.readLine();

        while (line != null && !line.startsWith(BEGIN)) {
            line = reader.readLine();
        }

        if (line != null) {
            line = line.substring(BEGIN.length());
            int index = line.indexOf('-');

            if (index > 0 && line.endsWith("-----") && (line.length() - index) == 5) {
                String type = line.substring(0, index);

                return loadContent(reader, type);
            }
        }

        return null;
    }

    static private byte[] loadContent(BufferedReader reader, String type) throws Exception {
        String line;
        String endMarker = END + type;
        StringBuffer buf = new StringBuffer();
        List headers = new ArrayList();

        while ((line = reader.readLine()) != null) {
            if (line.indexOf(":") >= 0) {
                int index = line.indexOf(':');
                String value = line.substring(index + 1).trim();
                continue;
            }

            if (line.indexOf(endMarker) != -1) {
                break;
            }

            buf.append(line.trim());
        }

        if (line == null) {
            throw new IOException(endMarker + " not found");
        }

        return Base64.decode(buf.toString(), 0);
    }
}
