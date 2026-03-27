package de.ullisroboterseite.ursai2pahomqtt;

/**
 * \brief Zustände der Verbindung mit dem Broker.
 */
public enum MqttConnectionState {
    Disconnected, // Der Client ist mit keinem Broker verbunden.
    Connecting, // Der Client baut die Verbindung zum Broker auf.
    Connected, // Der Client ist mit einem Broker verbunden.
    Disconnecting, // Der Client baut die Verbindung mit dem Broker ab.
    ConnectionAbortet // Die Verbindung konnte nicht aufgebaut werden bzw. wurde unterbrochen.
}