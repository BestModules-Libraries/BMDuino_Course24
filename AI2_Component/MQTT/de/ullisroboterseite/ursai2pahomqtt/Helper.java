package de.ullisroboterseite.ursai2pahomqtt;

import org.eclipse.paho.client.mqttv3.*;

public class Helper {
    // Zum Ausführen einer Methode in einem separatem Thread
    static class Runner {
        static void start(Runnable r) {
            Thread t1 = new Thread(r);
            t1.start();
        }
    }

    public static final short REASON_CODE_INVALID_STATE = 32300;
    public static final short REASON_CODE_EMPTY_TOPIC = 32301;
    public static final short REASON_CODE_CONVERT_ERROR = 32302;
    public static final short REASON_CODE_NOT_BYTE_ARRAY = 32303;
    public static final short REASON_CODE_INVALID_CON_PARAMETERS = 32304;
    public static final short REASON_CODE_INVALID_DICTIONARY = 32305;
    public static final short REASON_CODE_INVALID_TRUSTSTORE = 32306;
    public static final short REASON_CODE_INVALID_TRUSTEDCERT = 32307;
    public static final short REASON_CODE_INVALID_CLIENTCERT = 32308;
    public static final short REASON_CODE_INVALID_CLIENTKEYSTORE = 32309;

    // Gibt die Fehlertexte zu den Fehlercodes einer MqttException
    static String mqttException2Message(MqttException me) {
        switch (me.getReasonCode()) {
            case MqttException.REASON_CODE_CLIENT_EXCEPTION:
                return "Client encountered an exception.";

            case MqttException.REASON_CODE_INVALID_PROTOCOL_VERSION:
                return "Invalid protocol version.";

            case MqttException.REASON_CODE_INVALID_CLIENT_ID:
                return "Invalid client ID.";

            case MqttException.REASON_CODE_BROKER_UNAVAILABLE:
                return "Broker unavailable.";

            case MqttException.REASON_CODE_FAILED_AUTHENTICATION:
                return "Bad user name or password.";

            case MqttException.REASON_CODE_NOT_AUTHORIZED:
                return "Not authorized to connect.";

            case MqttException.REASON_CODE_UNEXPECTED_ERROR:
                return "Unexpected error.";

            case MqttException.REASON_CODE_SUBSCRIBE_FAILED:
                return "Error from subscribe.";

            case MqttException.REASON_CODE_CLIENT_TIMEOUT:
                return "Timed out waiting for a response.";

            case MqttException.REASON_CODE_NO_MESSAGE_IDS_AVAILABLE:
                return "No new message ID available.";

            case MqttException.REASON_CODE_WRITE_TIMEOUT:
                return "Timed out at writing.";

            case MqttException.REASON_CODE_CLIENT_CONNECTED:
                return "Already connected.";

            case MqttException.REASON_CODE_CLIENT_ALREADY_DISCONNECTED:
                return "Already disconnected.";

            case MqttException.REASON_CODE_CLIENT_DISCONNECTING:
                return "Currently disconnecting.";

            case MqttException.REASON_CODE_SERVER_CONNECT_ERROR:
                return "Unable to connect to server.";

            case MqttException.REASON_CODE_CLIENT_NOT_CONNECTED:
                return "Not connected.";

            case MqttException.REASON_CODE_SOCKET_FACTORY_MISMATCH:
                return "URI and SocketFactory do not match.";

            case MqttException.REASON_CODE_SSL_CONFIG_ERROR:
                return "SSL configuration error.";

            case MqttException.REASON_CODE_CLIENT_DISCONNECT_PROHIBITED:
                return "Disconnecting not allowed.";

            case MqttException.REASON_CODE_INVALID_MESSAGE:
                return "Unrecognized packet.";

            case MqttException.REASON_CODE_CONNECTION_LOST:
                return "Connection lost.";

            case MqttException.REASON_CODE_CONNECT_IN_PROGRESS:
                return "A connect already in progress.";

            case MqttException.REASON_CODE_CLIENT_CLOSED:
                return "The client is closed.";

            case MqttException.REASON_CODE_TOKEN_INUSE:
                return "Token already in use.";

            case MqttException.REASON_CODE_MAX_INFLIGHT:
                return "Too many publishes in progress.";

            // eigene Fehlermeldungen
            // ---------------------------------------------------
            case REASON_CODE_INVALID_STATE:
                return "Invalid State.";

            case REASON_CODE_EMPTY_TOPIC:
                return "Empty topic.";

            case REASON_CODE_CONVERT_ERROR:
                return "Invalid binary code.";

            case REASON_CODE_NOT_BYTE_ARRAY:
                return "Not a byte array.";

            case REASON_CODE_INVALID_CON_PARAMETERS:
                return "Invalid connection parameters.";

            case REASON_CODE_INVALID_DICTIONARY:
                return "Invalid dictionary content.";

            case REASON_CODE_INVALID_TRUSTSTORE:
                return "Cannot load truststore file.";

            case REASON_CODE_INVALID_TRUSTEDCERT:
                return "Cannot load trusted certificate file.";

            case REASON_CODE_INVALID_CLIENTCERT:
                return "Cannot load client certificate file or key file.";

            case REASON_CODE_INVALID_CLIENTKEYSTORE:
                return "Cannot load client keystore file.";

            default:
                return "Not a defined Mqtt reason code.";
        }
    }
}
