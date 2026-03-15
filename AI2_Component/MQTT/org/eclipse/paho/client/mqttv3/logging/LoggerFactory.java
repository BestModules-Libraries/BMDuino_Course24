/*******************************************************************************
 * Copyright (c) 2009, 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    https://www.eclipse.org/legal/epl-2.0
 * and the Eclipse Distribution License is available at
 *   https://www.eclipse.org/org/documents/edl-v10.php
 *
 * Contributors:
 *    Dave Locke - initial API and implementation and/or initial documentation
 */
package org.eclipse.paho.client.mqttv3.logging;

import java.lang.reflect.Method;

/**
 * LoggerFactory will create a logger instance ready for use by the caller.
 *
 * The default is to create a logger that utilises the Java's built in
 * logging facility java.util.logging (JSR47).  It is possible to override
 * this for systems where JSR47 is not available or an alternative logging
 * facility is needed by using setLogger and passing the the class name of
 * a logger that implements {@link Logger}
 */
import java.util.MissingResourceException;
import java.util.ResourceBundle;
/**
 * A factory that returns a logger for use by the MQTT client.
 *
 * The default log and trace facility uses Java's build in log facility:-
 * java.util.logging.  For systems where this is not available or where
 * an alternative logging framework is required the logging facility can be
 * replaced using {@link org.eclipse.paho.client.mqttv3.logging.LoggerFactory#setLogger(String)}
 * which takes an implementation of the {@link org.eclipse.paho.client.mqttv3.logging.Logger}
 * interface.
 */
public class LoggerFactory {
	/**
	 * Default message catalog.
	 */
	public final static String MQTT_CLIENT_MSG_CAT = "org.eclipse.paho.client.mqttv3.internal.nls.logcat";
	private static final String CLASS_NAME = LoggerFactory.class.getName();

	private static String overrideloggerClassName = null;
	/**
	 * Default logger that uses java.util.logging.
	 */
//	private static String jsr47LoggerClassName = JSR47Logger.class.getName();

	/**
	 * Find or create a logger for a named package/class.
	 * If a logger has already been created with the given name
	 * it is returned. Otherwise a new logger is created. By default a logger
	 * that uses java.util.logging will be returned.
	 *
	 * @param messageCatalogName the resource bundle containing the logging messages.
	 * @param loggerID  unique name to identify this logger.
	 * @return a suitable Logger.
	 */
	public static Logger getLogger(String messageCatalogName, String loggerID) {
		return new UrsLogger();
	}


	/**
	 * Return an instance of a logger
	 *
	 * @param the class name of the load to load
	 * @param messageCatalog  the resource bundle containing messages
	 * @param loggerID  an identifier for the logger
	 * @param resourceName a name or context to associate with this logger instance.
	 * @return a ready for use logger
	 */
	private static Logger getLogger(String loggerClassName, ResourceBundle messageCatalog, String loggerID, String resourceName) { //, FFDC ffdc) {
		return new UrsLogger();
	}

	/**
	 * When run in JSR47, this allows access to the properties in the logging.properties
	 * file.
	 * If not run in JSR47, or the property isn't set, returns null.
	 * @param name the property to return
	 * @return the property value, or null if it isn't set or JSR47 isn't being used
	 */
	public static String getLoggingProperty(String name) {
		String result = null;

		return result;
	}

	/**
	 * Set the class name of the logger that the LoggerFactory will load
	 * If not set getLogger will attempt to create a logger
	 * appropriate for the platform.
	 * @param loggerClassName - Logger implementation class name to use.
	 */
	public static void setLogger(String loggerClassName) {
		LoggerFactory.overrideloggerClassName = loggerClassName;
	}
}