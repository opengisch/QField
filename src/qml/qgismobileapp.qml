/***************************************************************************
                            qgismobileapp.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import QtQml 2.2
import org.qgis 1.0
import org.qfield 1.0
import QtPositioning 5.4
import "js/style.js" as Style

import '.'

ApplicationWindow {
  id: mainWindow
  visible: true
  minimumWidth: 600
  minimumHeight: 400

  Item {
    id: stateMachine

    states: [
      State {
        name: "browse"
      },

      State {
        name: "digitize"
      }
    ]
    state: "browse"
  }

  /**
   * The position source to access the GPS
   */
  PositionSource {
    id: positionSource
    // active: true
    active: settings.valueBool( "/QField/Positioning/Active", false )

    onPositionChanged: {
      var coord = positionSource.position.coordinate;
      locationMarker.location = Qt.point( coord.longitude, coord.latitude )
    }
  }

  Item {
    /*
     * This is the map canvas
     * On top of it are the base map and other items like GPS icon...
     */
    id: mapCanvas

    /* Initialize a MapSettings object. This will contain information about
     * the current canvas extent. It is shared between the base map and all
     * map canvas items and is used to transform map coordinates to pixel
     * coordinates.
     * It may change any time and items that hold a reference to this property
     * are responsible to handle this properly.
     */
    property MapSettings mapSettings: mapCanvasMap.mapSettings

    /* Placement and size. Share right anchor with featureForm */
    anchors.fill: parent

    /* The base map */
    MapCanvas {
      id: mapCanvasMap

      anchors.fill: parent

      onClicked: {
        identifyTool.identify( Qt.point( mouse.x, mouse.y ) )
        featureForm.show()
      }
    }

  /**************************************************
   * Position markers
   * - Coordinate Locator
   * - GPS Marker
   * - Identify Highlight
   * - Digitizing Rubberband
   **************************************************/

    /* A transformation node for overlays in map coordinates */
    Item {
      anchors.fill: parent

      transform: MapTransform {
        mapSettings: mapCanvas.mapSettings
      }

      /* Highlight the currently selected item on the feature list */
      FeatureListModelHighlight {
        model: featureForm.model
        selection: featureForm.selection
        mapSettings: mapCanvas.mapSettings
        color: "yellow"
        selectionColor: "#ff7777"
        width: 5 * dp
      }

      /** A rubberband for ditizing **/
      Rubberband {
        id: digitizingRubberband
        width: 2 * dp

        mapSettings: mapCanvas.mapSettings

        model: RubberbandModel {
          currentCoordinate: coordinateLocator.snappedCoordinate
          vectorLayer: dashBoard.currentLayer
          crs: mapCanvas.mapSettings.destinationCrs
        }

        anchors.fill: parent

        visible: stateMachine.state === "digitize"
      }

      /** The identify tool **/
      IdentifyTool {
        id: identifyTool

        mapSettings: mapCanvas.mapSettings
        model: featureForm.model
      }
    }

    /** A coordinate locator for digitizing **/
    CoordinateLocator {
      id: coordinateLocator
      anchors.fill: parent
      visible: stateMachine.state === "digitize"
      highlightColor: digitizingToolbar.isDigitizing ? digitizingRubberband.color : "#CFD8DC"
      mapSettings: mapCanvas.mapSettings
      currentLayer: dashBoard.currentLayer
    }

    /* GPS marker  */
    LocationMarker {
      id: locationMarker
      mapSettings: mapCanvas.mapSettings
      coordinateTransform: CoordinateTransform {
        sourceCRS: CrsFactory.fromEpsgId(4326)
        destinationCRS: mapCanvas.mapSettings.destinationCrs
      }
      anchors.fill: parent
      visible: positionSource.active
    }
  }

  /**************************************************
   * Map Canvas Decorations like
   * - Position Information View
   * - Scale Bar
   **************************************************/

  ScaleBar {
    visible: qfieldSettings.showScaleBar
    mapSettings: mapCanvas.mapSettings

    anchors.left: parent.left
    anchors.bottom: parent.bottom
  }

  Item {
    id: positionInformationView
    anchors.right: featureForm.left
    anchors.top: parent.top
    visible: settings.valueBool( "/QField/Positioning/ShowInformationView", false )

    width: childrenRect.width
    height: childrenRect.height

    Rectangle {
      color: "white"
      opacity: 0.7
      anchors.fill: parent
      radius: 5
    }

    Rectangle {
      color: "transparent"
      border.color: "gray"
      anchors.fill: parent
      radius: 5
    }

    PositionInformationView {
      positionSource: positionSource
      crs: mapCanvas.mapSettings.destinationCrs

      anchors.margins: 5
    }
  }

  DropShadow {
    anchors.fill: featureForm
    horizontalOffset: -2 * dp
    verticalOffset: 0
    radius: 6.0 * dp
    samples: 17
    color: "#80000000"
    source: featureForm
  }

  DashBoard {
    id: dashBoard
    anchors { left: parent.left; bottom: parent.bottom; top: parent.top; }

    width: 0
    clip: true

    allowLayerChange: !digitizingToolbar.isDigitizing
    mapSettings: mapCanvas.mapSettings

    Behavior on width {
      NumberAnimation {
        duration: 200
        easing.type: Easing.InOutQuad
      }
    }

    onShowMenu: mainMenu.popup()

    onCurrentLayerChanged: {
      if ( currentLayer.readOnly && stateMachine.state == "digitize" )
        displayToast( qsTr( "The layer %1 is read only." ).arg( currentLayer.name ) )
    }
  }

  DropShadow {
    anchors.fill: dashBoard
    horizontalOffset: 2 * dp
    verticalOffset: 0
    radius: 6.0 * dp
    samples: 17
    color: "#80000000"
    source: dashBoard
  }

  /* The main menu */
  Row {
    id: mainMenuBar
    height: childrenRect.height

    Button {
      iconSource: Style.getThemeIcon( "ic_menu_white_24dp" )
      onClicked: {
        if ( dashBoard.width > 0 )
        {
          dashBoard.width = 0
          bgcolor = "#212121"
          rotate()
        }
        else
        {
          dashBoard.width = 300 * dp
          bgcolor = "#C8E6C9"
          rotate()
        }
      }
    }
  }

  Button {
    id: gpsButton
    state: positionSource.active ? "On" : "Off"
    visible: positionSource.valid
    round: true

    anchors.left: dashBoard.right
    anchors.leftMargin: 4 * dp
    anchors.top: mainMenuBar.bottom
    anchors.topMargin: 4 * dp

    bgcolor: "#64B5F6"

    states: [
      State {

        name: "Off"
        PropertyChanges {
          target: gpsButton
          iconSource: Style.getThemeIcon( "ic_location_disabled_white_24dp" )
        }
      },

      State {
        name: "On"
        PropertyChanges {
          target: gpsButton
          iconSource: positionSource.position.latitudeValid ? Style.getThemeIcon( "ic_my_location_white_24dp" ) : Style.getThemeIcon( "ic_gps_not_fixed_white_24dp" )
        }
      }
    ]

    onClicked: {
      if ( positionSource.position.latitudeValid )
      {
        var coord = positionSource.position.coordinate;
        var loc = Qt.point( coord.longitude, coord.latitude );
        mapCanvas.mapSettings.setCenter( locationMarker.coordinateTransform.transform( loc ) )

        if ( !positionSource.active )
        {
          positionSource.active = true;
          displayToast( qsTr( "Activating positioning service..." ) )
        }
      }
      else
      {
        if ( positionSource.valid )
        {
          if ( positionSource.active )
          {
            displayToast( qsTr( "Waiting for location..." ) )
          }
          else
          {
            positionSource.active = true
            displayToast( qsTr( "Activating positioning service..." ) )
          }
        }
      }
    }

    onPressAndHold: {
      gpsMenu.popup()
    }

    function toggleGps() {
      switch ( gpsButton.state )
      {
        case "Off":
          gpsButton.state = "On"
          displayToast( qsTr( "Positioning activated" ) )
          break;

        case "On":
          gpsButton.state = "Off"
          displayToast( qsTr( "Positioning turned off" ) )
          break;
      }
    }
  }

  DigitizingToolbar {
    id: digitizingToolbar

    anchors.bottom: mapCanvas.bottom
    anchors.right: mapCanvas.right

    visible: ( stateMachine.state === "digitize" && !dashBoard.currentLayer.readOnly )
    rubberbandModel: digitizingRubberband.model

    FeatureModel {
      id: digitizingFeature
      currentLayer: dashBoard.currentLayer

      geometry: Geometry {
        rubberbandModel: digitizingRubberband.model
        vectorLayer: dashBoard.currentLayer
      }
    }

    onVertexAdded: {
      coordinateLocator.flash()
      digitizingRubberband.model.addVertex()
    }

    onVertexRemoved:
    {
      digitizingRubberband.model.removeVertex()
    }

    onCancel:
    {
      digitizingRubberband.model.reset()
    }

    onConfirm: {
      coordinateLocator.flash()

      digitizingFeature.geometry.applyRubberband()
      digitizingFeature.applyGeometry()

      if ( !digitizingFeature.suppressFeatureForm() )
      {
        digitizingFeature.resetAttributes();
        overlayFeatureForm.visible = true;
        overlayFeatureForm.state = "Add"
        digitizingRubberband.model.reset()
      }
      else
      {
        digitizingFeature.create()
        digitizingRubberband.model.reset()
      }
    }
  }

  Controls.Menu {
    id: mainMenu
    title: qsTr( "Main Menu" )


    Controls.Menu {
      title: qsTr( "Mode" )

      Controls.MenuItem {
        text: qsTr( "Browse" )
        onTriggered: stateMachine.state = "browse"
      }

      Controls.MenuItem {
        text: qsTr( "Digitize" )
        onTriggered: stateMachine.state = "digitize"
      }
    }

    Controls.MenuItem {
      text: qsTr( "Open Project" )
      iconSource: Style.getThemeIcon( "ic_map_white_24dp" )
      onTriggered: {
        openProjectDialog.visible = true
      }
    }

    Controls.MenuSeparator {}

    Controls.MenuItem {
      text: qsTr( "Settings" )

      onTriggered: {
        qfieldSettings.visible = true
      }
    }

    Controls.MenuItem {
      text: qsTr( "About" )

      onTriggered: {
        aboutDialog.visible = true
      }
    }

    Controls.MenuItem {
      text: qsTr( "Log" )

      onTriggered: {
        messageLog.visible = true
      }
    }

    Controls.MenuSeparator {}

    Controls.MenuItem {
      text: qsTr( "Quit" )
      iconSource: Style.getThemeIcon( "ic_close_white_24dp" )
      onTriggered: {
        Qt.quit()
      }
    }
  }

  Controls.Menu {
    id: gpsMenu
    title: qsTr( "GPS Options" )

    Controls.MenuItem {
      text: qsTr( "Enable GPS" )
      checkable: true
      checked: positionSource.active
      onCheckedChanged: {
        positionSource.active = checked
      }
    }

    Controls.MenuItem {
      text: qsTr( "Center current location" )
      onTriggered: {
        var coord = positionSource.position.coordinate;
        var loc = Qt.point( coord.longitude, coord.latitude );
        mapCanvas.mapSettings.setCenter( locationMarker.coordinateTransform.transform( loc ) )
      }
    }

    Controls.MenuSeparator {}

    Controls.MenuItem {
      text: qsTr( "Show position information" )
      checkable: true
      checked: settings.valueBool( "/QField/Positioning/ShowInformationView", false )
      onCheckedChanged:
      {
        settings.setValue( "/QField/Positioning/ShowInformationView", checked )
        positionInformationView.visible = checked
      }
    }
  }


  Rectangle {
    id: overlayBackground

    anchors.fill: parent

    visible: overlayFeatureForm.visible

    color: "white"
  }

  Image {
    id: alertIcon
    source: Style.getThemeIcon( "ic_add_alert_black_18dp" )

    visible: messageLog.unreadMessages

    anchors.right: mapCanvas.right
    anchors.top: parent.top
    anchors.margins: 4 * dp
    width: 36 * dp
    height: 36 * dp

    MouseArea {
      anchors.fill: parent
      onClicked: messageLog.visible = true
    }
  }

  /* The feature form */
  FeatureListForm {
    id: featureForm
    mapSettings: mapCanvas.mapSettings

    anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
    border { color: "lightGray"; width: 1 }
    allowDelete: stateMachine.state === "digitize"

    model: MultiFeatureListModel {}

    selection: FeatureListModelSelection {
      model: featureForm.model
    }

    selectionColor: "#ff7777"
  }

  FeatureForm {
    id: overlayFeatureForm

    anchors.fill: parent

    model: AttributeFormModel {
      featureModel: digitizingFeature
    }

    state: "Add"

    visible: false

    onSaved: {
      visible = false
      if ( state === "Add" )
        digitizingRubberband.model.reset()
    }
    onCancelled: visible = false
  }

  function displayToast( message ) {
    toastMessage.text = message
    toast.opacity = 1
  }

  Rectangle {
    id: busyMessage
    anchors.fill: parent
    color: "#272727"
    opacity: 0.5
    visible: false

    Controls.BusyIndicator {
      id: busyMessageIndicator
      anchors.centerIn: parent
      running: true
    }

    Text {
      id: busyMessageText
      anchors.top: busyMessageIndicator.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      text: qsTr( "Loading Project" )
    }

    Connections {
      target: iface

      onLoadProjectStarted: {
        busyMessageText.text = qsTr( "Loading Project: %1" ).arg( path )
        busyMessage.visible = true
      }

      onLoadProjectEnded: {
        busyMessage.visible = false
        openProjectDialog.folder = qgisProject.homePath
      }
    }
  }

  Controls.BusyIndicator {
    id: busyIndicator
    anchors { right: alertIcon.left; top: parent.top }
    width: 36 * dp
    height: 36 * dp
    running: mapCanvasMap.isRendering
  }

  MessageLog {
    id: messageLog
    anchors.fill: parent

    model: MessageLogModel {}

    visible: false

    Keys.onReleased: {
      if (event.key === Qt.Key_Back) {
        event.accepted = true
        visible = false
      }
    }
    onVisibleChanged: focus = true
  }

  BadLayerItem {
    id: badLayersView

    anchors.fill: parent
    model: BadLayerHandler {
      project: qgisProject

      onBadLayersFound: {
        badLayersView.visible = true
      }
    }

    visible: false

    onFinished: {
      visible = false
    }
  }

  About {
    id: aboutDialog
    anchors.fill: parent

    visible: false
  }

  FileDialog {
    id: openProjectDialog
    title: qsTr( "Open project" )
    visible: false
    nameFilters: [ qsTr( "QGIS projects (*.qgs)" ), qsTr( "All files (*)" ) ]

    width: parent.width
    height: parent.height

    onAccepted: {
      iface.loadProject( openProjectDialog.fileUrl.toString().slice(7) )
    }
  }

  QFieldSettings {
    id: qfieldSettings

    anchors.fill: parent
    visible: false

    onFinished: {
      visible = false
    }
  }

  WelcomeScreen {
    id: welcomeScreen
    anchors.fill: parent
    visible: !settings.value( "/QField/FirstRunFlag", false )

    onShowOpenProjectDialog: {
      welcomeScreen.visible = false
      openProjectDialog.visible = true
    }
  }

  // Toast
  Rectangle {
    id: toast
    anchors.horizontalCenter: parent.horizontalCenter
    color: "#272727"
    height: 40*dp;
    width: ( (toastMessage.width + 16*dp) <= 192*dp ) ? 192*dp : toastMessage.width + 16*dp
    opacity: 0
    radius: 20*dp
    y: parent.height - 112*dp
    z: 1
    Behavior on opacity {
      NumberAnimation { duration: 500 }
    }

    // Visible only for 3 seconds
    onOpacityChanged: {
      toast.visible = (toast.opacity > 0)
      if ( toast.opacity == 1 ) {
        toastTimer.start()
      }
    }

    Text {
      id: toastMessage
      anchors.centerIn: parent
      font.pixelSize: 16*dp
      color: "#ffffff"
    }
      // Visible only for 3 seconds
    Timer {
      id: toastTimer
      interval: 3000
      onTriggered: { toast.opacity = 0 }
    }
  }

  DropArea {
    id: dropArea
    anchors.fill: parent
    onEntered: {
      if ( drag.urls.length !== 1
         || !validateFileExtension( drag.urls[0] ) ) {
          drag.accepted = false
      }
      else {
        drag.accept (Qt.CopyAction)
        drag.accepted = true
      }
    }
    onDropped: {
      iface.loadProject( drop.urls[0] )
    }

    function validateFileExtension(filePath) {
      return filePath.split('.').pop() === "qgs"
    }
  }

}
