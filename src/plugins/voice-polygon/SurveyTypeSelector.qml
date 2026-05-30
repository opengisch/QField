// src/plugins/voice-polygon/SurveyTypeSelector.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

QfDialog {
  id: root
  title: "¿Qué vas a levantar?"
  width: Math.min(iface.mainWindow().width * 0.9, 480)
  modal: true

  signal typeSelected(string type)

  ColumnLayout {
    anchors.fill: parent
    spacing: 12

    Text {
      Layout.fillWidth: true
      text: "Selecciona el tipo de elemento para configurar la capa de destino y el estilo del polígono."
      color: Theme.secondaryTextColor
      font.pixelSize: 13
      wrapMode: Text.Wrap
    }

    SurveyTypeCard {
      Layout.fillWidth: true
      typeId: "construccion"
      icon: "🏠"
      title: "Construcción"
      description: "Planta arquitectónica de una edificación.\nCaptura muros, puertas, ventanas y columnas."
      cardColor: "#1565C0"
      targetLayer: "construcciones.gpkg"
      onSelected: root.typeSelected("construccion")
    }

    SurveyTypeCard {
      Layout.fillWidth: true
      typeId: "unidad"
      icon: "🏗️"
      title: "Unidad de construcción"
      description: "Local, departamento o área funcional dentro de una construcción."
      cardColor: "#2E7D32"
      targetLayer: "unidades.gpkg"
      onSelected: root.typeSelected("unidad")
    }

    SurveyTypeCard {
      Layout.fillWidth: true
      typeId: "terreno"
      icon: "📐"
      title: "Terreno / Linderos"
      description: "Perímetro del predio. Traza colindancias y cierres de lindero."
      cardColor: "#E65100"
      targetLayer: "predios.gpkg"
      onSelected: root.typeSelected("terreno")
    }

    QfButton {
      Layout.fillWidth: true
      text: "Cancelar"
      flat: true
      onClicked: root.close()
    }
  }

  component SurveyTypeCard: Rectangle {
    property string typeId
    property string icon
    property string title
    property string description
    property color  cardColor
    property string targetLayer

    signal selected()

    height: 90
    radius: 10
    color: cardMouse.containsMouse
      ? Qt.rgba(cardColor.r, cardColor.g, cardColor.b, 0.15)
      : Qt.rgba(cardColor.r, cardColor.g, cardColor.b, 0.08)
    border.color: cardColor
    border.width: 1.5

    RowLayout {
      anchors.fill: parent
      anchors.margins: 14
      spacing: 14

      Text { text: icon; font.pixelSize: 36 }

      ColumnLayout {
        Layout.fillWidth: true
        spacing: 2

        Text { text: title; color: cardColor; font.pixelSize: 16; font.bold: true }
        Text { text: description; color: Theme.secondaryTextColor; font.pixelSize: 11;
               wrapMode: Text.Wrap; Layout.fillWidth: true }
        Text { text: "→ " + targetLayer; color: Theme.secondaryTextColor;
               font.pixelSize: 10; font.italic: true }
      }

      Text { text: "›"; color: cardColor; font.pixelSize: 28 }
    }

    MouseArea {
      id: cardMouse
      anchors.fill: parent
      hoverEnabled: true
      onClicked: parent.selected()
    }
  }
}
