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
import Theme 1.0

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
          mainWindow.closeMeasureTool()
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

  signal closeMeasureTool()
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

  onCloseMeasureTool: {
    overlayFeatureFormDrawer.close()
    changeMode( stateMachine.lastState)
  }

  /**
   * The position source to access the GPS
   */
  TransformedPositionSource {
    id: positionSource
    active: settings.valueBool( "/QField/Positioning/Active", false )
    destinationCrs: mapCanvas.mapSettings.destinationCrs
    deltaZ: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight * -1 : 0
    skipAltitudeTransformation: positioningSettings.skipAltitudeCorrection
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
      color: mapCanvas.mapSettings.backgroundColor
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

      onPanned: {
          if ( gpsButton.followActive )
          {
            gpsButton.followActive = false
            displayToast( qsTr( "Canvas stopped following location" ) )
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

      /** Tracking session for each layer **/
      TrackingModel{
          id: trackingModel
          activated: positionSource.active && stateMachine.state === "digitize"
          onTrackerStarted: {
              displayToast( qsTr( 'Track on layer %1 started' ).arg( layer.name  ) )
          }
          onTrackerStopped: {
              displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layer.name  ) )
          }
      }
      Repeater {
          id: trackings
          model: trackingModel
          Tracking {
          }
      }

      /** The identify tool **/
      IdentifyTool {
        id: identifyTool

        mapSettings: mapCanvas.mapSettings
        model: featureForm.model
      }

      /** A rubberband for the different geometry editors **/
      Rubberband {
        id: geometryEditorsRubberband
        width: 2 * dp
        color: '#80000000'

        mapSettings: mapCanvas.mapSettings

        model: RubberbandModel {
          frozen: false
          currentCoordinate: coordinateLocator.currentCoordinate
          crs: mapCanvas.mapSettings.destinationCrs
          geometryType: QgsWkbTypes.LineGeometry
        }

        anchors.fill: parent
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
      accuracy: positionSource.projectedHorizontalAccuracy
      direction: positionSource.position.directionValid ? positionSource.position.direction : -1

      onLocationChanged: {
        if ( gpsButton.followActive ) {
          var screenLocation = mapSettings.coordinateToScreen( location );
          var screenFraction = settings.value( "/QField/Positioning/FollowScreenFraction", 5 );
          var threshold = Math.min( mainWindow.width, mainWindow.height ) / screenFraction;
          if ( screenLocation.x < threshold || screenLocation.x > mainWindow.width - threshold ||
               screenLocation.y < threshold || screenLocation.y > mainWindow.height - threshold )
          {
            mapCanvas.mapSettings.setCenter(positionSource.projectedPosition);
          }
        }
      }
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

  Item {
    id: positionInformationView
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    visible: settings.valueBool( "/QField/Positioning/ShowInformationView", false )

    height: childrenRect.height
    width: parent.width

    PositionInformationView {
      id: p
      positionSource: positionSource
      antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : NaN
    }
  }

  DropShadow {
    anchors.fill: positionInformationView
    visible: positionInformationView.visible
    verticalOffset: -2 * dp
    radius: 6.0 * dp
    samples: 17
    color: "#30000000"
    source: positionInformationView
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
                .arg(stateMachine.state === 'digitize' || !digitizingToolbar.isDigitizing ? '<p>%1: %2<br>%3: %4</p>'
                  .arg(coordinateLocator.mapSettings.destinationCrs.isGeographic ? qsTr( 'Lon' ) : 'X')
                  .arg(coordinateLocator.currentCoordinate.x.toFixed( coordinateLocator.mapSettings.destinationCrs.isGeographic ? 5 : 2 ))
                  .arg(coordinateLocator.mapSettings.destinationCrs.isGeographic ? qsTr( 'Lat' ) : 'Y')
                  .arg(coordinateLocator.currentCoordinate.y.toFixed( coordinateLocator.mapSettings.destinationCrs.isGeographic ? 5 : 2 ))
                  : '' )

                .arg(digitizingGeometryMeasure.lengthValid ? '<p>%1: %2%3</p>'
                  .arg( digitizingGeometryMeasure.segmentLength != digitizingGeometryMeasure.length ? qsTr( 'Segment') : qsTr( 'Length') )
                  .arg(UnitTypes.formatDistance( digitizingGeometryMeasure.segmentLength, 3, digitizingGeometryMeasure.lengthUnits ) )
                  .arg(digitizingGeometryMeasure.length !== -1 && digitizingGeometryMeasure.segmentLength != digitizingGeometryMeasure.length ? '<br>%1: %2'.arg( qsTr( 'Length') ).arg(UnitTypes.formatDistance( digitizingGeometryMeasure.length, 3, digitizingGeometryMeasure.lengthUnits ) ) : '' )
                  : '' )

                .arg(digitizingGeometryMeasure.areaValid ? '<p>%1: %2</p>'
                  .arg( qsTr( 'Area') )
                  .arg(UnitTypes.formatArea( digitizingGeometryMeasure.area, 3, digitizingGeometryMeasure.areaUnits ) )
                  : '' )

                .arg(stateMachine.state === 'measure' && digitizingToolbar.isDigitizing? '<p>%1: %2<br>%3: %4</p>'
                  .arg(coordinateLocator.mapSettings.destinationCrs.isGeographic ? qsTr( 'Lon' ) : 'X')
                  .arg(coordinateLocator.currentCoordinate.x.toFixed( coordinateLocator.mapSettings.destinationCrs.isGeographic ? 5 : 2 ))
                  .arg(coordinateLocator.mapSettings.destinationCrs.isGeographic ? qsTr( 'Lat' ) : 'Y')
                  .arg(coordinateLocator.currentCoordinate.y.toFixed( coordinateLocator.mapSettings.destinationCrs.isGeographic ? 5 : 2 ))
                  : '' )
              : ''

    font: Theme.strongFont
    style: Text.Outline
    styleColor: Theme.light
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

    width: Math.max( 200 * dp, mainWindow.width / 3 )
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: 4 * dp

    visible: stateMachine.state !== 'measure'
  }

  DashBoard {
    id: dashBoard
    allowLayerChange: !digitizingToolbar.isDigitizing
    mapSettings: mapCanvas.mapSettings
    interactive: !welcomeScreen.visible
  }

  /* The main menu */
  Row {
    id: mainMenuBar
    width: childrenRect.width + 8 * dp
    height: childrenRect.height + 8 * dp
    topPadding: 4 * dp
    leftPadding: 4 * dp
    spacing: 4 * dp

    Button {
      id: menuButton
      round: true
      iconSource: Theme.getThemeIcon( "ic_menu_white_24dp" )
      onClicked: dashBoard.opened ? dashBoard.close() : dashBoard.open()
      bgcolor: dashBoard.opened ? Theme.mainColor : Theme.darkGray
    }

    CloseTool {
      id: closeMeasureTool
      visible: stateMachine.state === 'measure'
      toolText: qsTr( 'Close measure tool' )
      onClosedTool: mainWindow.closeMeasureTool()
    }

    CloseTool {
      id: closeGeometryEditorsTool
      visible: ( stateMachine.state === "digitize" && vertexModel.vertexCount > 0 )
      toolText: qsTr( 'Stop editing' )
      onClosedTool: geometryEditorsToolbar.cancelEditors()
    }
  }

  Column {
    id: mainToolBar
    anchors.right: mapCanvas.right
    anchors.rightMargin: 4 * dp
    anchors.bottom: mapCanvas.bottom
    anchors.bottomMargin: 4 * dp
    spacing: 4 * dp

    Button {
      id: gpsLinkButton
      visible: gpsButton.state == "On" && ( stateMachine.state === "digitize" || stateMachine.state === 'measure' )
      round: true
      checkable: true
      anchors.right: parent.right

      bgcolor: Theme.darkGray
      iconSource: linkActive ? Theme.getThemeIcon( "ic_gps_link_activated_white_24dp" ) : Theme.getThemeIcon( "ic_gps_link_white_24dp" )

      readonly property bool linkActive: gpsButton.state == "On" && checked

      onClicked: gpsLinkButton.checked = !gpsLinkButton.checked
    }

    Button {
      id: gpsButton
      state: positionSource.active ? "On" : "Off"
      visible: positionSource.valid
      round: true
      anchors.right: parent.right

      bgcolor: "#64B5F6"

      property bool followActive: false

      states: [
        State {

          name: "Off"
          PropertyChanges {
            target: gpsButton
            iconSource: Theme.getThemeIcon( "ic_location_disabled_white_24dp" )
            bgcolor: "#88212121"
          }
        },

        State {
          name: "On"
          PropertyChanges {
            target: gpsButton
            iconSource: positionSource.position.latitudeValid ? Theme.getThemeIcon( "ic_my_location_" + ( followActive ? "white" : "blue" ) + "_24dp" ) : Theme.getThemeIcon( "ic_gps_not_fixed_white_24dp" )
            bgcolor: followActive ? "#64B5F6" : Theme.darkGray
            opacity:1
          }
        }
      ]

      onClicked: {
        followActive = true
        if ( positionSource.projectedPosition.x )
        {
          if ( !positionSource.active )
          {
            positionSource.active = true;
            displayToast( qsTr( "Activating positioning service" ) )
          }
          else
          {
            mapCanvas.mapSettings.setCenter(positionSource.projectedPosition)
            displayToast( qsTr( "Canvas follows location" ) )
          }
        }
        else
        {
          if ( positionSource.valid )
          {
            if ( positionSource.active )
            {
              displayToast( qsTr( "Waiting for location" ) )
            }
            else
            {
              positionSource.active = true
              displayToast( qsTr( "Activating positioning service" ) )
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

      stateVisible: (stateMachine.state === "digitize"
                     && dashBoard.currentLayer
                     && !dashBoard.currentLayer.readOnly
                     && !geometryEditorsToolbar.stateVisible ) || stateMachine.state === 'measure'
      rubberbandModel: currentRubberband.model
      coordinateLocator: coordinateLocator
      mapSettings: mapCanvas.mapSettings
      showConfirmButton: stateMachine.state === "digitize"

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

    GeometryEditorsToolbar {
      id: geometryEditorsToolbar

      featureModel: geometryEditingFeature
      mapSettings: mapCanvas.mapSettings
      editorRubberbandModel: geometryEditorsRubberband.model

      stateVisible: ( stateMachine.state === "digitize" && vertexModel.vertexCount > 0 )
    }
  }


  Menu {
    id: mainMenu
    title: qsTr( "Main Menu" )

    width: Math.max(200*dp, mainWindow.width/4)

    MenuItem {
      id: openProjectMenuItem

      font: Theme.defaultFont
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      text: qsTr( "Open Project" )
      onTriggered: {
        dashBoard.close()
        welcomeScreen.visible = true
        welcomeScreen.focus = true
        highlighted = false
      }
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( "Settings" )

      font: Theme.defaultFont
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.close()
        qfieldSettings.visible = true
        highlighted = false
      }
    }

    MenuItem {
      text: qsTr( "About" )

      font: Theme.defaultFont
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.close()
        aboutDialog.visible = true
        highlighted = false
      }
    }

    MenuItem {
      text: qsTr( "Log" )

      font: Theme.defaultFont
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.close()
        messageLog.visible = true
        highlighted = false
      }
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( 'Measure Tool' )

      font: Theme.defaultFont
      width: parent.width
      height: 48 * dp
      leftPadding: 10 * dp

      onTriggered: {
        dashBoard.close()
        changeMode( 'measure' )
        highlighted = false
      }
    }

    MenuItem {
      id: printItem
      text: qsTr( "Print to PDF" )

      font: Theme.defaultFont
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
      iconSource: Theme.getThemeIcon( "ic_close_white_24dp" )
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
    font: Theme.defaultFont

    Instantiator {

      id: layoutListInstantiator

      model: PrintLayoutListModel {
      }

      MenuItem {
        text: Title

        width: parent.width
        height: 48 * dp
        font: Theme.defaultFont
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
        welcomeScreen.visible = false
        welcomeScreen.focus = false
        recentProjectListModel.reloadModel()
        settings.setValue( "/QField/FirstRunFlag", false )
      }
    }
  }

  Menu {
    id: gpsMenu
    title: qsTr( "Positioning Options" )
    font: Theme.defaultFont
    width: Math.max(200*dp, mainWindow.width/1.5)

    MenuItem {
      text: qsTr( "Enable Positioning" )

      height: 48 * dp
      font: Theme.defaultFont
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

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( "Center to Current Location" )

      height: 48 * dp
      font: Theme.defaultFont
      width: parent.width
      onTriggered: {
        var coord = positionSource.position.coordinate;
        var loc = Qt.point( coord.longitude, coord.latitude );
        mapCanvas.mapSettings.setCenter( locationMarker.coordinateTransform.transform( loc ) )
      }
    }

    MenuSeparator { width: parent.width }

    MenuItem {
      text: qsTr( "Show Position Information" )

      height: 48 * dp
      font: Theme.defaultFont
      width: parent.width
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

    MenuItem {
      text: qsTr( "Configure Antenna Height" ) // Todo: rename to "Positioning Configuration" when there is more to configure
      height: 48 * dp
      font: Theme.defaultFont
      width: parent.width

      onTriggered: {
        positioningSettings.visible = true
      }
    }
  }

  Button {
    id: alertIcon
    iconSource: Theme.getThemeIcon( "ic_alert_black_24dp" )
    round: true
    bgcolor: "transparent"

    visible: messageLog.unreadMessages

    anchors.right: locatorItem.right
    anchors.top: locatorItem.bottom
    anchors.topMargin: 5 * dp

    onClicked: messageLog.visible = true
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

      geometryEditorsToolbar.init()
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

  function displayToast( message ) {
    //toastMessage.text = message
    toast.show(message)
  }

  Rectangle {
    id: busyMessage
    anchors.fill: parent
    color: Theme.darkGray
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
    anchors.left: mainMenuBar.left
    anchors.top: mainMenuBar.bottom
    padding: 5 * dp
    width: mainMenuBar.height
    height: mainMenuBar.height
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

    onFinished: {
      visible = false
    }

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
      padding: 0
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

  PositioningSettings {
    id: positioningSettings
    visible: false

    x: 24 * dp
    y: 24 * dp
    width: parent.width - 48 * dp
    height: parent.height - 48 * dp
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
    model: RecentProjectListModel {
      id: recentProjectListModel
    }
    property ProjectSource __projectSource

    anchors.fill: parent
    focus: true

    visible: true

    onShowOpenProjectDialog: {
      __projectSource = platformUtilities.openProject()
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        console.log(focus)
        if ( qgisProject.fileName != '') {
          event.accepted = true
          visible = false
          focus = false
        } else {
          event.accepted = false
          mainWindow.close()
        }
      }
    }

    Component.onCompleted: {
        focusstack.addFocusTaker( this )
    }
  }

  FileDialog {
    id: openProjectDialog
    title: qsTr( "Open project" )
    visible: false
    nameFilters: [ qsTr( "QGIS projects (*.qgs *.qgz)" ), qsTr( "All files (*)" ) ]

    width: mainWindow.width
    height: mainWindow.height

    onAccepted: {
      iface.loadProject( openProjectDialog.fileUrl.toString().slice(7) )
      mainWindow.keyHandler.focus=true
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
      closePolicy: Popup.NoAutoClose

      background: Rectangle { color: "transparent" }

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
        color: Theme.darkGray

        height: toastMessage.height
        width: toastMessage.text.length * 16 * dp <= 192 * dp ? 192 * dp : ( toastMessage.text.length * 16 * dp ) - 16 * dp > mainWindow.width ? mainWindow.width - 16 * dp : toastMessage.text.length * 16 * dp

        anchors.centerIn: parent

        radius: 16 * dp

        z: 1

        Text {
          id: toastMessage
          anchors.left: parent.left
          anchors.right: parent.right
          wrapMode: Text.Wrap

          font: Theme.secondaryTitleFont
          horizontalAlignment: Text.AlignHCenter
          color: Theme.light
        }

        MouseArea {
          anchors.fill: parent
          propagateComposedEvents: true
          onClicked: {
              mouse.accepted = false
          }
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

