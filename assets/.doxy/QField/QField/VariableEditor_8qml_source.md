

# File VariableEditor.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**VariableEditor.qml**](VariableEditor_8qml.md)

[Go to the documentation of this file](VariableEditor_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

ColumnLayout {
  function reset() {
    Qt.inputMethod.hide();
    table.model.reloadVariables();
  }

  function apply() {
    table.model.save();
  }

  anchors.fill: parent
  Layout.fillWidth: true
  Layout.fillHeight: true
  spacing: 4

  Rectangle {
    Layout.fillWidth: true
    Layout.fillHeight: true
    color: Theme.mainBackgroundColor
    border.color: Theme.controlBorderColor
    border.width: 1

    ListView {
      id: table
      flickableDirection: Flickable.VerticalFlick
      boundsBehavior: Flickable.StopAtBounds
      clip: true
      spacing: 1
      anchors.fill: parent

      ScrollBar.vertical: QfScrollBar {}

      model: ExpressionVariableModel {
        currentProject: qgisProject
      }

      section.property: "VariableScope"
      section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
      section.delegate: Component {
        Rectangle {
          width: parent.width
          height: 30
          color: Theme.controlBorderColor

          Text {
            anchors {
              horizontalCenter: parent.horizontalCenter
              verticalCenter: parent.verticalCenter
            }
            font.bold: true
            font.pointSize: Theme.resultFont.pointSize
            color: Theme.mainTextColor
            text: section == "GlobalScope" ? qsTr("Global variables") : qsTr("Project variables")
          }
        }
      }

      delegate: Rectangle {
        id: rectangle
        width: parent ? parent.width : 0
        height: line.height + 10
        color: "transparent"

        property var itemRow: index
        property bool canDelete: VariableEditable && VariableScope == ExpressionVariableModel.GlobalScope

        function forceFocusOnVariableName() {
          variableNameText.forceActiveFocus();
        }

        Column {
          id: line
          anchors {
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
          }
          spacing: 0

          QfSwipeAnimator {
            id: variableNameTextAnimator
            width: table.width - line.anchors.leftMargin * 2
            height: 24
            shouldAutoFlick: (width < variableNameText.implicitWidth) && !dragging && !variableNameText.activeFocus
            contentImplicitWidth: variableNameText.implicitWidth
            contentWidth: variableNameText.implicitWidth
            duration: shouldAutoFlick ? Math.abs(variableNameText.width - width) * 100 + 10 : 10000

            TextField {
              id: variableNameText
              topPadding: 5
              bottomPadding: 0
              leftPadding: 1
              rightPadding: 1
              width: Math.max(table.width - line.anchors.leftMargin * 2, implicitWidth)
              height: variableNameTextAnimator.height
              text: VariableName
              enabled: VariableScope == ExpressionVariableModel.GlobalScope && VariableEditable
              font.bold: true
              font.pointSize: Theme.tinyFont.pointSize
              color: Theme.mainTextColor
              horizontalAlignment: TextInput.AlignLeft
              verticalAlignment: TextInput.AlignVCenter

              background: Rectangle {
                color: "transparent"
              }

              onTextChanged: {
                if (enabled && VariableName != text) {
                  VariableName = text;
                }
              }

              onCursorRectangleChanged: {
                variableNameTextAnimator.ensureCursorVisible(cursorRectangle);
              }
            }
          }

          Row {
            spacing: 5

            QfSwipeAnimator {
              id: variableValueTextAnimator
              width: table.width - line.anchors.leftMargin * 2 - (canDelete ? deleteVariableButton.width : 0)
              height: variableValueText.height
              shouldAutoFlick: (width < variableValueText.implicitWidth) && !dragging && !variableValueText.activeFocus
              contentImplicitWidth: variableValueText.implicitWidth
              contentWidth: variableValueText.implicitWidth
              duration: shouldAutoFlick ? Math.abs(variableValueText.width - width) * 100 + 10 : 10000

              QfTextField {
                id: variableValueText
                topPadding: 10
                bottomPadding: 10
                rightPadding: 1
                width: Math.max(table.width - line.anchors.leftMargin * 2 - (canDelete ? deleteVariableButton.width : 0), implicitWidth)
                text: VariableValue
                enabled: VariableEditable
                font: Theme.defaultFont
                horizontalAlignment: TextInput.AlignLeft
                placeholderText: !variableValueText.activeFocus && displayText === '' ? qsTr("Enter value") : ''
                color: Theme.mainTextColor

                onTextChanged: {
                  if (enabled && VariableValue != text) {
                    VariableValue = text;
                    if (VariableScope == ExpressionVariableModel.ProjectScope) {
                      projectInfo.saveVariable(VariableName, text);
                    }
                  }
                }

                background.visible: enabled
                onCursorRectangleChanged: {
                  variableValueTextAnimator.ensureCursorVisible(cursorRectangle);
                }
              }
            }

            QfToolButton {
              id: deleteVariableButton
              width: 36
              height: 36
              anchors.verticalCenter: parent.verticalCenter
              visible: canDelete

              iconSource: Theme.getThemeVectorIcon('ic_delete_forever_white_24dp')
              iconColor: Theme.mainTextColor
              bgcolor: "transparent"

              onClicked: {
                table.model.removeVariable(VariableScope, variableNameText.text);
              }
            }
          }
        }
      }
    }
  }

  QfButton {
    id: addCustomVariableButton
    Layout.fillWidth: true
    text: qsTr("Add a new variable")

    onClicked: {
      let insertionPosition = table.model.addVariable(ExpressionVariableModel.GlobalScope, "new_variable", "");
      table.positionViewAtIndex(insertionPosition, ListView.Contain);
      table.itemAtIndex(insertionPosition).forceFocusOnVariableName();
    }
  }
}
```


