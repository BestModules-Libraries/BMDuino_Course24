package org.eclipse.paho.client.mqttv3;

// Dummy Komponente. Erzwingt das Kompilieren mit "ant extensions"


import com.google.appinventor.components.annotations.*;
import com.google.appinventor.components.common.*;
import com.google.appinventor.components.runtime.*;


import org.eclipse.paho.client.mqttv3.*;


@DesignerComponent(version = 1, //
        versionName = "1.0", //
        dateBuilt = "2020-10-20", //
        description = "Dummy.", //
        category = com.google.appinventor.components.common.ComponentCategory.EXTENSION, //
        nonVisible = true)
@SimpleObject(external = true)
public class UrsDummyComponent extends AndroidNonvisibleComponent { //implements IExtensionListener {
    public UrsDummyComponent(ComponentContainer container) {
        super(container.$form());
     }
}