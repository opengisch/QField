import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
  property var layerTree
  property var index
  property var panToLayerButtonText

    property var trackingButtonVisible: isTrackingButtonVisible()
    property var trackingButtonBgColor
    property var trackingButtonText

  property alias itemVisible: itemVisibleCheckBox.checked

  signal panToLayerButtonClicked
  signal trackingButtonClicked

  padding: 0

  x: (parent.width - width) / 2
  y: (parent.height - height) / 2

  onIndexChanged: {
    itemVisible = layerTree.data(index, FlatLayerTreeModel.Visible)
    title = qsTr("%1 : Properties and Functions").arg(layerTree.data(index, 0))
    trackingButtonVisible = isTrackingButtonVisible()
    trackingButtonText = trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ? qsTr('Stop tracking') : qsTr('Start tracking')
  }

  onItemVisibleChanged: {
    layerTree.setData(index, itemVisible, FlatLayerTreeModel.Visible);
  }

  onPanToLayerButtonClicked: {
    mapCanvas.mapSettings.setCenterToLayer( layerTree.data( index, FlatLayerTreeModel.MapLayerPointer ) )
  }

  onTrackingButtonClicked: {
      //start track
      if( trackingModel.layerInTracking( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer) ) ) {
          trackingModel.stopTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer));
          displayToast( qsTr( 'Track on layer %1 stopped' ).arg( layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer).name  ) )
      }else{
          trackingModel.createTracker(layerTree.data(index, FlatLayerTreeModel.VectorLayerPointer), itemVisible);
      }
      close()
  }


  Page {
    padding: 10
    header: Label {
      padding: 10
      topPadding: 20
      bottomPadding: 5
      anchors.left:parent.left
      anchors.right:parent.right
      text: title
      font: Theme.strongFont
    }

    ColumnLayout{
      spacing: 4
      width: Math.min(mainWindow.width - 20, parent.width)

      Text {
        id: text
        visible: layerTree.data(index, FlatLayerTreeModel.ReadOnly) || layerTree.data(index, FlatLayerTreeModel.GeometryLocked)
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20

        wrapMode: Text.WordWrap
        text: layerTree.data(index, FlatLayerTreeModel.ReadOnly)
              ? qsTr('This layer is configured as "Read-Only" which disables adding, deleting and editing features.')
              : qsTr('This layer is configured as "Lock Geometries" which disables adding and deleting features, as well as modifying the geometries of existing features.')
      }

      CheckBox {
          id: itemVisibleCheckBox
          Layout.fillWidth: true
          Layout.maximumWidth: parent.width - 20
          text: qsTr('Show on map canvas')
          font: Theme.defaultFont

          indicator.height: 16
          indicator.width: 16
          indicator.implicitHeight: 24
          indicator.implicitWidth: 24
      }

      QfButton {
        id: panToLayerButton
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20
        font: Theme.defaultFont
        text: qsTr('Zoom to layer')

        onClicked: {
          panToLayerButtonClicked()
        }
      }

      QfButton {
        id: trackingButton
        Layout.fillWidth: true
        Layout.maximumWidth: parent.width - 20
        font: Theme.defaultFont
        text: trackingButtonText
        visible: trackingButtonVisible

        onClicked: {
          trackingButtonClicked()
        }
      }
    }
  }


  function isTrackingButtonVisible() {
    if ( ! index )
      return false

    return layerTree.data(index, FlatLayerTreeModel.Type) === 'layer'
        && layerTree.data(index, FlatLayerTreeModel.Trackable)
        && positionSource.active
  }
}
