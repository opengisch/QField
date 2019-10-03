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

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0 as LabSettings
import QtQml 2.2
import org.qgis 1.0
import org.qfield 1.0
import QtPositioning 5.11
import "js/style.js" as Style

import '.'

ApplicationWindow {
  id: mainWindow
  visible: true

  LabSettings.Settings {
      property alias x: mainWindow.x
      property alias y: mainWindow.y
      property alias width: mainWindow.width
      property alias height: mainWindow.height
  }

  FocusStack{
      id: focusstack
  }

  //this keyHandler is because otherwise the back-key is not handled in the mainWindow. Probably this could be solved cuter.

  Item {
    id: keyHandler
    objectName: "keyHandler"

    visible: true
    focus: true

    Keys.onReleased: {
      if ( event.key === Qt.Key_Back ||
        event.key === Qt.Key_Escape ) {
        if ( stateMachine.state === 'measure' ) {
          closeTool()
        } else {
          mainWindow.close();
        }
        event.accepted = true
      }
    }

    Component.onCompleted: focusstack.addFocusTaker( this )
  }

  //currentRubberband provides the rubberband depending on the current state (digitize or measure)
  property Rubberband currentRubberband

  signal closeTool()
  signal changeMode( string mode )

  Item {
    id: stateMachine

    property string lastState

    states: [
      State {
        name: "browse"
        PropertyChanges { target: identifyTool; deactivated: false }
      },

      State {
        name: "digitize"
        PropertyChanges { target: identifyTool; deactivated: false }
        PropertyChanges { target: mainWindow; currentRubberband: digitizingRubberband }
      },

      State {
        name: 'measure'
        PropertyChanges { target: identifyTool; deactivated: true }
        PropertyChanges { target: mainWindow; currentRubberband: measuringRubberband }
        PropertyChanges { target: dashBoard; visible: false }
        PropertyChanges { target: featureForm; state: "Hidden" }
      }
    ]
    state: "browse"
  }

  onChangeMode: {
    stateMachine.lastState = stateMachine.state
    stateMachine.state = mode
    displayToast( qsTr( 'You are now in %1 mode ' ).arg( stateMachine.state  ) )
  }

  onCloseTool: {
    changeMode( stateMachine.lastState)
  }

  /**
   * The position source to access the GPS
   */
  TransformedPositionSource {
    id: positionSource
    active: settings.valueBool( "/QField/Positioning/Active", false )
    destinationCrs: mapCanvas.mapSettings.destinationCrs
  }

  Item {
    id: positionInformationView
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    visible: settings.valueBool( "/QField/Positioning/ShowInformationView", false )

    height: childrenRect.height
    width: parent.width

    PositionInformationView {
      positionSource: positionSource
      crs: mapCanvas.mapSettings.destinationCrs
    }
  }

  Item {
    /*
     * This is the map canvas
     * On top of it are the base map and other items like GPS icon...
     */
    id: mapCanvas
    clip: true

    /* Initialize a MapSettings object. This will contain information about
     * the current canvas extent. It is shared between the base map and all
     * map canvas items and is used to transform map coordinates to pixel
     * coordinates.
     * It may change any time and items that hold a reference to this property
     * are responsible to handle this properly.
     */
    property MapSettings mapSettings: mapCanvasMap.mapSettings

    /* Placement and size. Share right anchor with featureForm */
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: positionInformationView.visible ? positionInformationView.top : parent.bottom

    Rectangle {
      id: mapCanvasBackground
      anchors.fill: parent
      color: mapSettings.backgroundColor
    }

    /* The base map */
    MapCanvas {

      id: mapCanvasMap
      incrementalRendering: qfieldSettings.incrementalRendering

      anchors.fill: parent

      onClicked: {
          if (locatorItem.searching) {
              locatorItem.searching = false
          } else if( !overlayFeatureFormDrawer.visible ) {
              identifyTool.identify( Qt.point( mouse.x, mouse.y ) )
          }
      }

      Component.onCompleted: platformUtilities.showRateThisApp()
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

      /** A rubberband for ditizing **/
      Rubberband {
        id: digitizingRubberband
        width: 2 * dp

        mapSettings: mapCanvas.mapSettings

        model: RubberbandModel {
          frozen: false
          currentCoordinate: coordinateLocator.currentCoordinate
          vectorLayer: dashBoard.currentLayer
          crs: mapCanvas.mapSettings.destinationCrs

          onCurrentCoordinateChanged: {
              console.info( currentCoordinate.x+', '+currentCoordinate.y+', '+currentCoordinate.z )
          }
        }

        anchors.fill: parent

        visible: stateMachine.state === "digitize"
      }

      /** A rubberband for measuring **/
      Rubberband {
        id: measuringRubberband
        width: 2 * dp
        color: '#80000000'

        mapSettings: mapCanvas.mapSettings

        model: RubberbandModel {
          frozen: false
          currentCoordinate: coordinateLocator.currentCoordinate
          geometryType: QgsWkbTypes.PolygonGeometry
          crs: mapCanvas.mapSettings.destinationCrs
        }

        anchors.fill: parent

        visible: stateMachine.state === 'measure'
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
      visible: stateMachine.state === "digitize" || stateMachine.state === 'measure'
      highlightColor: digitizingToolbar.isDigitizing ? currentRubberband.color : "#CFD8DC"
      mapSettings: mapCanvas.mapSettings
      currentLayer: dashBoard.currentLayer
      overrideLocation: gpsLinkButton.linkActive ? positionSource.projectedPosition : undefined
    }

    /* GPS marker  */
    LocationMarker {
      id: locationMarker
      mapSettings: mapCanvas.mapSettings
      anchors.fill: parent
      visible: positionSource.active
      location: positionSource.projectedPosition
    }

    /* Rubberband for vertices  */
    Item {
      VertexRubberband {
        id: vertexRubberband
        model: vertexModel
        mapSettings: mapCanvas.mapSettings
      }

      Rubberband {
        id: editingRubberBand
        vertexModel: vertexModel
        mapSettings: mapCanvas.mapSettings
        width: 2 * dp

        transform: MapTransform {
          mapSettings: mapCanvas.mapSettings
        }
      }
    }

    /* Locator Highlight */
    GeometryHighlighter {
      id: locatorHighlightItem
      //width: 10 * dp
      //color: "yellow"
    }

    /* Highlight the currently selected item on the feature list */
    FeatureListSelectionHighlight {
      selectionModel: featureForm.selection
      mapSettings: mapCanvas.mapSettings

      //model: featureForm.model
      //selection: featureForm.selection

      color: "yellow"
      selectionColor: "#ff7777"
      width: 5 * dp
    }
  }

  /**************************************************
   * Map Canvas Decorations like
   * - Position Information View
   * - Scale Bar
   **************************************************/

  Text {
    DistanceArea {
      id: digitizingGeometryMeasure

      property VectorLayer currentLayer: dashBoard.currentLayer

      rubberbandModel: currentRubberband.model
      project: qgisProject
      crs: qgisProject.crs
    }

    x: mainWindow.width / 2 + 24 * dp
    y: mainWindow.height / 2 + 24 * dp

    text: ( qfieldSettings.numericalDigitizingInformation && stateMachine.state === "digitize" ) || stateMachine.state === 'measure' ?
              '%1%2%3%4'
                .arg(stateMachine.state === 'digitize' || !digitizingToolbar.isDigitizing ? '<p>%1 / %2</p>'
                  .arg(coordinateLocator.currentCoordinate.x.toFixed(3))
                  .arg(coordinateLocator.currentCoordinate.y.toFixed(3))
                  : '' )

                .arg(digitizingGeometryMeasure.lengthValid ? '<p>%1 %2</p>'
                  .arg(UnitTypes.formatDistance( digitizingGeometryMeasure.segmentLength, 3, digitizingGeometryMeasure.lengthUnits ) )
                  .arg(digitizingGeometryMeasure.length !== -1 ? '(%1)'.arg(UnitTypes.formatDistance( digitizingGeometryMeasure.length, 3, digitizingGeometryMeasure.lengthUnits ) ) : '' )
                  : '' )

                .arg(digitizingGeometryMeasure.areaValid ? '<p>%1</p>'
                  .arg(UnitTypes.formatArea( digitizingGeometryMeasure.area, 3, digitizingGeometryMeasure.areaUnits ) )
                  : '' )

                .arg(stateMachine.state === 'measure' && digitizingToolbar.isDigitizing? '<p>%1 / %2</p>'
                  .arg(coordinateLocator.currentCoordinate.x.toFixed(3))
                  .arg(coordinateLocator.currentCoordinate.y.toFixed(3))
                  : '' )
              : ''

    font.pointSize: 12
    style: Text.Outline
    font.weight: Font.Bold
    styleColor: "white"
  }

  ScaleBar {
    visible: qfieldSettings.showScaleBar
    mapSettings: mapCanvas.mapSettings

    anchors.left: mapCanvas.left
    anchors.bottom: mapCanvas.bottom
    anchors.margins: 10 * dp
  }

  DropShadow {
    anchors.fill: featureForm
    horizontalOffset: mainWindow.width >= mainWindow.height ? -2 * dp : 0
    verticalOffset: mainWindow.width < mainWindow.height ? -2 * dp : 0
    radius: 6.0 * dp
    samples: 17
    color: "#80000000"
    source: featureForm
  }

  LocatorItem {
    id: locatorItem

    width: mainWindow.width < 300 * dp ? mainWindow.width - anchors.margins - mainMenuBar.width : 200 * dp
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: 10 * dp

    visible: stateMachine.state !== 'measure'
  }

  DashBoard {
    id: dashBoard
    allowLayerChange: !digitizingToolbar.isDigitizing
    mapSettings: mapCanvas.mapSettings
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
    width: childrenRect.width + 8 * dp
    height: childrenRect.height + 8 * dp

    Button {
      id: menuButton
      round: true
      iconSource: Style.getThemeIcon( "ic_menu_white_24dp" )
      onClicked: dashBoard.visible = !dashBoard.visible
      bgcolor: dashBoard.visible ? "#80CC28" : "#212121"
      anchors.left: mainMenuBar.left
      anchors.leftMargin: 4 * dp
      anchors.top: mainMenuBar.top
      anchors.topMargin: 4 * dp
    }

    ToolButton {
      id: closeMeasureTool
      height: 48 * dp
      width: height + buttonText.width + 32 * dp

      contentItem: Rectangle {
        anchors.fill: parent
        color: '#80000000'
        radius: height / 2

        Row {
          spacing: 8 * dp
          Rectangle {
            height: 48 * dp
            width: 48 * dp
            radius: height / 2
            color: '#212121'
            Image {
              anchors.fill: parent
              fillMode: Image.Pad
              horizontalAlignment: Image.AlignHCenter
              verticalAlignment: Image.AlignVCenter
              source: Style.getThemeIcon( "ic_close_white_24dp" )
            }
          }

          Text {
            id: buttonText
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            text: qsTr( 'Close measure tool' )
            color: 'white'
            font.bold: true
            font.pixelSize: 16 * dp
          }
        }

        Behavior on color {
          ColorAnimation {
            duration: 200
          }
        }
      }
      visible: stateMachine.state === 'measure'
      onClicked: {
        overlayFeatureFormDrawer.close()
        closeTool()
      }
      anchors.left: mainMenuBar.left
      anchors.leftMargin: 4 * dp
      anchors.top: mainMenuBar.top
      anchors.topMargin: 4 * dp
    }
  }

  Column {
    id: mainToolBar
    anchors.right: mapCanvas.right
    anchors.rightMargin: 4 * dp
    anchors.bottom: mapCanvas.bottom
    anchors.bottomMargin: digitizingToolbar.height + 4 * dp
    spacing: 4 * dp

    Button {
      id: gpsLinkButton
      visible: gpsButton.state == "On" && ( stateMachine.state === "digitize" || stateMachine.state === 'measure' )
      round: true
      bgcolor: "#212121"
      checkable: true

      iconSource: linkActive ? Style.getThemeIcon( "ic_gps_link_activated_white_24dp" ) : Style.getThemeIcon( "ic_gps_link_white_24dp" )

      readonly property bool linkActive: gpsButton.state == "On" && checked

      onClicked: gpsLinkButton.checked = !gpsLinkButton.checked
    }

    Button {
      id: gpsButton
      state: positionSource.active ? "On" : "Off"
      visible: positionSource.valid
      round: true

      bgcolor: "#64B5F6"

      states: [
        State {

          name: "Off"
          PropertyChanges {
            target: gpsButton
            iconSource: Style.getThemeIcon( "ic_location_disabled_white_24dp" )
            bgcolor: "#88212121"
          }
        },

        State {
          name: "On"
          PropertyChanges {
            target: gpsButton
            iconSource: positionSource.position.latitudeValid ? Style.getThemeIcon( "ic_my_location_white_24dp" ) : Style.getThemeIcon( "ic_gps_not_fixed_white_24dp" )
            bgcolor: "#64B5F6"
            opacity:1
          }
        }
      ]

      onClicked: {
        if ( positionSource.projectedPosition.x )
        {
          mapCanvas.mapSettings.setCenter(positionSource.projectedPosition)

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
  }

  DigitizingToolbar {
    id: digitizingToolbar

    anchors.bottom: mapCanvas.bottom
    anchors.right: mapCanvas.right

    stateVisible: (stateMachine.state === "digitize"
                   && dashBoard.currentLayer
                   && !dashBoard.currentLayer.readOnly
                   && !geometryEditingToolbar.stateVisible ) || stateMachine.state === 'measure'
    rubberbandModel: currentRubberband.model

    FeatureModel {
      id: digitizingFeature
      currentLayer: dashBoard.currentLayer
      positionSourceName: positionSource.name
      topSnappingResult: coordinateLocator.topSnappingResult

      geometry: Geometry {
        id: digitizingGeometry
        rubberbandModel: digitizingRubberband.model
        vectorLayer: dashBoard.currentLayer
      }
    }

    onVertexAdded: {
      coordinateLocator.flash()
      currentRubberband.model.addVertex()
    }

    onVertexRemoved:
    {
      currentRubberband.model.removeVertex()
      mapCanvas.mapSettings.setCenter( currentRubberband.model.currentCoordinate )
    }

    onCancel:
    {
      currentRubberband.model.reset()
    }

    onConfirm: {
      if (digitizingRubberband.model.geometryType === QgsWkbTypes.NullGeometry )
      {
        digitizingRubberband.model.reset()
      }
      else
      {
        coordinateLocator.flash()
        digitizingFeature.geometry.applyRubberband()
        digitizingFeature.applyGeometry()
        digitizingRubberband.model.frozen = true
      }

      if ( !digitizingFeature.suppressFeatureForm() )
      {
        digitizingFeature.resetAttributes();
        overlayFeatureFormDrawer.open()
        overlayFeatureFormDrawer.state = "Add"
        overlayFeatureFormDrawer.featureForm.reset()
      }
      else
      {
        digitizingFeature.create()
        digitizingRubberband.model.reset()
      }
    }
  }

  GeometryEditingToolbar {
    id: geometryEditingToolbar

    featureModel: geometryEditingFeature
    mapSettings: mapCanvas.mapSettings

    anchors.bottom: mapCanvas.bottom
    anchors.right: mapCanvas.right

    stateVisible: ( stateMachine.state === "digitize" && vertexModel.vertexCount > 0 )
  }


  Menu {
    id: mainMenu
    title: qsTr( "Main Menu" )

    width: Math.max(200*dp, mainWindow.width/4)

    MenuItem {
      id: openProjectMenuItem
      property ProjectSource __projectSource

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      text: qsTr( "Open Project" )
      onTriggered: {
        __projectSource = platformUtilities.openProject()
        highlighted = false
      }
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( "Settings" )

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.visible = false
        qfieldSettings.visible = true
        highlighted = false
      }
    }

    MenuItem {
      text: qsTr( "About" )

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.visible = false
        aboutDialog.visible = true
        highlighted = false
      }
    }

    MenuItem {
      text: qsTr( "Log" )

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.visible = false
        messageLog.visible = true
        highlighted = false
      }
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( 'Measure Tool' )

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        changeMode( 'measure' )
        highlighted = false
      }
    }

    MenuItem {
      id: printItem
      text: qsTr( "Print to PDF" )

      font.pointSize: 14
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        printMenu.popup( mainMenu.x, 2 * dp )
        highlighted = false
      }
    }

    Connections {
        target: printMenu

        onEnablePrintItem: {
          printItem.enabled = rows
        }
    }

    /*
    We removed this MenuItem part, because usually a mobile app has not the functionality to quit.
    But we keep the code in case, the concept changes or we need to close the app completely (remove from background)
    */

    /*
    Controls.MenuSeparator {}

    Controls.MenuItem {
      text: qsTr( "Quit" )
      iconSource: Style.getThemeIcon( "ic_close_white_24dp" )
      onTriggered: {
        Qt.quit()
      }
    }
    */
  }

  Menu {
    id: printMenu

    title: qsTr( "Print to PDF" )

    signal enablePrintItem( int rows )

    width: Math.max(200*dp, mainWindow.width/4)
    font.pointSize: 14

    Instantiator {

      id: layoutListInstantiator

      model: PrintLayoutListModel {
      }

      MenuItem {
        text: Title

        width: parent.width
        height: 48 * dp
        font.pointSize: 14
        leftPadding: 10 * dp

        onTriggered: {
          iface.print( Index )
          highlighted = false
        }
      }
      onObjectAdded: printMenu.insertItem(index, object)
      onObjectRemoved: printMenu.removeItem(object)
    }

    Connections {
      target: iface

      onLoadProjectEnded: {
        layoutListInstantiator.model.project = qgisProject
        layoutListInstantiator.model.reloadModel()
        printMenu.enablePrintItem(layoutListInstantiator.model.rowCount())
      }
    }
  }

  Menu {
    id: gpsMenu
    title: qsTr( "Positioning Options" )
    font.pointSize: 14
    width: Math.max(200*dp, mainWindow.width/4)

    MenuItem {
      text: qsTr( "Enable Positioning" )

      height: 48 * dp
      font.pointSize: 14
      width: parent.width
      checkable: true
      checked: positionSource.active
      indicator.height: 20 * dp
      indicator.width: 20 * dp
      indicator.implicitHeight: 24 * dp
      indicator.implicitWidth: 24 * dp

      onCheckedChanged: {
        if ( checked && platformUtilities.checkPositioningPermissions() ) {
          positionSource.active = checked
        }
        else {
          displayToast( qsTr( "QField has no permissions to use positioning." ) )
          positionSource.active = false
        }

      }
    }

    MenuSeparator { width: Math.max(200*dp, mainWindow.width/4) }

    MenuItem {
      text: qsTr( "Center current location" )

      height: 48 * dp
      font: Theme.defaultFont
      width: Math.max(200*dp, mainWindow.width/4)
      onTriggered: {
        var coord = positionSource.position.coordinate;
        var loc = Qt.point( coord.longitude, coord.latitude );
        mapCanvas.mapSettings.setCenter( locationMarker.coordinateTransform.transform( loc ) )
      }
    }

    MenuSeparator { width: Math.max(200*dp, mainWindow.width/4) }

    MenuItem {
      text: qsTr( "Show position information" )

      height: 48 * dp
      font.pointSize: 14
      width: Math.max(200*dp, mainWindow.width/4)
      checkable: true
      checked: settings.valueBool( "/QField/Positioning/ShowInformationView", false )

      indicator.height: 20 * dp
      indicator.width: 20 * dp
      indicator.implicitHeight: 24 * dp
      indicator.implicitWidth: 24 * dp
      onCheckedChanged:
      {
        settings.setValue( "/QField/Positioning/ShowInformationView", checked )
        positionInformationView.visible = checked
      }
    }
  }

  Image {
    id: alertIcon
    source: Style.getThemeIcon( "ic_add_alert_black_18dp" )

    visible: messageLog.unreadMessages

    anchors.right: mapCanvas.right
    anchors.top: locatorItem.bottom
    anchors.topMargin: 10 * dp
    anchors.rightMargin: 15 * dp
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
    objectName: "featureForm"
    mapSettings: mapCanvas.mapSettings

    visible: state != "Hidden"
    focus: visible

    anchors { right: parent.right; bottom: parent.bottom }
    border { color: "lightGray"; width: 1 }
    allowEdit: stateMachine.state === "digitize"

    model: MultiFeatureListModel {}

    selection: FeatureListModelSelection {
      id: featureListModelSelection
      model: featureForm.model
    }

    selectionColor: "#ff7777"

    onShowMessage: displayToast(message)

    onEditGeometry: {
      vertexModel.geometry = featureForm.selection.selectedGeometry
      vertexModel.crs = featureForm.selection.selectedLayer.crs
      geometryEditingFeature.currentLayer = featureForm.selection.selectedLayer
      geometryEditingFeature.feature = featureForm.selection.selectedFeature

      if (!vertexModel.editingAllowed)
      {
        displayToast( qsTr( "Editing of multi geometry layer is not supported yet." ) )
        vertexModel.clear()
      }
      else
      {
        featureForm.state = "Hidden"
      }
    }

    Component.onCompleted: focusstack.addFocusTaker( this )

    //that the focus is set by selecting the empty space
    MouseArea {
      anchors.fill: parent
      propagateComposedEvents: true
      enabled: !parent.activeFocus

      //onPressed because onClicked shall be handled in underlying MouseArea
      onPressed: {
        parent.focus=true
        mouse.accepted=false
      }
    }
  }

  OverlayFeatureFormDrawer {
    id: overlayFeatureFormDrawer
    featureModel: digitizingFeature
  }

  Keys.onReleased: {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true
    }
  }

  function displayToast( message ) {
    //toastMessage.text = message
    toast.show(message)
  }

  Rectangle {
    id: busyMessage
    anchors.fill: parent
    color: "#272727"
    opacity: 0.5
    visible: false

    BusyIndicator {
      id: busyMessageIndicator
      anchors.centerIn: parent
      running: true
      width: 100 * dp
      height: 100 * dp
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
        mapCanvasBackground.color = mapCanvas.mapSettings.backgroundColor
      }
    }
  }

  BusyIndicator {
    id: busyIndicator
    anchors.centerIn: mapCanvas
    width: 36 * dp
    height: 36 * dp
    running: mapCanvasMap.isRendering
  }

  MessageLog {
    id: messageLog
    anchors.fill: parent
    focus: visible

    model: MessageLogModel {
      id: messageLogModel
    }

    visible: false

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true
        visible = false
      }
    }

    Component.onCompleted: focusstack.addFocusTaker( this )
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

  Item {
    id: layerLogin

    Connections {
      target: iface
      onLoadProjectEnded: {
        if( !qfieldAuthRequestHandler.handleLayerLogins() )
        {
          //project loaded without more layer handling needed
          messageLogModel.unsuppressTags(["WFS","WMS"])
        }
      }
    }
    Connections {
        target: iface

        onLoadProjectStarted: {
          messageLogModel.suppressTags(["WFS","WMS"])
        }
    }

    Connections {
      target: qfieldAuthRequestHandler

      onShowLoginDialog: {
        loginDialogPopup.realm = realm
        badLayersView.visible = false
        loginDialogPopup.open()
      }

      onReloadEverything: {
        iface.reloadProject( qgisProject.fileName )
      }
    }

    Popup {
      id: loginDialogPopup
      parent: ApplicationWindow.overlay

      property var realm

      x: 24 * dp
      y: 24 * dp
      width: parent.width - 48 * dp
      height: parent.height - 48 * dp
      modal: true
      closePolicy: Popup.CloseOnEscape

      LayerLoginDialog {
        id: loginDialog

        anchors.fill: parent

        visible: true

        realm: loginDialogPopup.realm
        inCancelation: false

        onEnter: {
          qfieldAuthRequestHandler.enterCredentials( realm, usr, pw)
          inCancelation = false;
          loginDialogPopup.close()
        }
        onCancel: {
          inCancelation = true;
          loginDialogPopup.close(true)
        }
      }

      onClosed: {
        //it's handeled here with parameter inCancelation because the loginDialog needs to be closed before the signal is fired
        qfieldAuthRequestHandler.loginDialogClosed(loginDialog.realm, loginDialog.inCancelation )
      }
    }

  }

  About {
    id: aboutDialog
    anchors.fill: parent
    focus: visible

    visible: false

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true
        visible = false
      }
    }

    Component.onCompleted: focusstack.addFocusTaker( this )
  }

  FileDialog {
    id: openProjectDialog
    title: qsTr( "Open project" )
    visible: false
    nameFilters: [ qsTr( "QGIS projects (*.qgs *.qgz)" ), qsTr( "All files (*)" ) ]

    width: parent.width
    height: parent.height

    onAccepted: {
      iface.loadProject( openProjectDialog.fileUrl.toString().slice(7) )
      mainWindow.keyHandler.focus=true
    }
  }

  QFieldSettings {
    id: qfieldSettings

    anchors.fill: parent
    visible: false
    focus: visible

    onFinished: {
      visible = false
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true
        finished()
      }
    }

    Component.onCompleted: focusstack.addFocusTaker( this )
  }

  WelcomeScreen {
    id: welcomeScreen
    anchors.fill: parent
    visible: !settings.value( "/QField/FirstRunFlag", false )
    property ProjectSource __projectSource

    onShowOpenProjectDialog: {
      welcomeScreen.visible = false
      __projectSource = platformUtilities.openProject()
    }
  }

  Popup {
    id: changelogPopup
    parent: ApplicationWindow.overlay

    property var expireDate: new Date(2019,9,16)
    visible: settings.value( "/QField/CurrentVersion", "" ) !== versionCode
               && expireDate > new Date()

    x: 24 * dp
    y: 24 * dp
    width: parent.width - 48 * dp
    height: parent.height - 48 * dp
    padding: 0
    modal: true
    closePolicy: Popup.CloseOnEscape

    Flickable {
      id: changelogFlickable
      anchors.fill: parent
      flickableDirection: Flickable.VerticalFlick
      interactive: true
      contentWidth: changelog.width; contentHeight: changelog.height
      clip: true

      Changelog {
        id: changelog
        width: changelogFlickable.width

        onClose: {
          changelogPopup.close()
        }
      }
    }
  }

  // Toast
  Popup {
      id: toast
      opacity: 0
      height: 40 * dp;
      width: parent.width
      y: parent.height - 112 * dp
      margins: 0
      background: undefined
      closePolicy: Popup.NoAutoClose

      function show(text) {
          toastMessage.text = text
          toast.open()
          toastContent.visible = true
          toast.opacity = 1
          toastTimer.start()
      }

      Behavior on opacity {
        NumberAnimation { duration: 500 }
      }

      Rectangle {
          id: toastContent
        color: "#272727"

        height: 40 * dp
        width: ( (toastMessage.width + 16 * dp) <= 192 * dp ) ? 192 * dp : toastMessage.width + 16 * dp

        anchors.centerIn: parent

        radius: 20 * dp

        z: 1

        Text {
          id: toastMessage
          anchors.centerIn: parent
          font.pixelSize: 16 * dp
          color: "#ffffff"
        }
      }

      // Visible only for 3 seconds
      Timer {
          id: toastTimer
          interval: 3000
          onTriggered: {
              toast.opacity = 0
          }
      }

      onOpacityChanged: {

          if ( opacity == 0 ) {
              toastContent.visible = false
              toast.close()
          }
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
      return filePath.split('.').pop() === "qgs" || filePath.split('.').pop() === "qgz"
    }
  }

  property bool alreadyCloseRequested: false

  onClosing: {
      if( !alreadyCloseRequested )
      {
        close.accepted = false
        alreadyCloseRequested = true
        displayToast( qsTr( "Press back again to close project and app" ) )
        closingTimer.start()
      }
      else
      {
        close.accepted = true
      }
  }

  Timer {
    id: closingTimer
    interval: 2000
    onTriggered: {
        alreadyCloseRequested = false
    }
  }

  Connections {
    target: openProjectMenuItem.__projectSource

    onProjectOpened: {
      iface.loadProject( path )
    }
  }

  Connections {
    target: welcomeScreen.__projectSource

    onProjectOpened: {
      iface.loadProject( path )
    }
  }

  // ! MODELS !
  FeatureModel {
    id: geometryEditingFeature
    currentLayer: null
    positionSourceName: positionSource.name
    vertexModel: vertexModel
  }

  VertexModel {
      id: vertexModel
      currentPoint: coordinateLocator.currentCoordinate
      mapSettings: mapCanvas.mapSettings
  }
}

