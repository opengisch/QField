

# File QfVariableEditor.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfVariableEditor.qml**](QfVariableEditor_8qml.md)

[Go to the documentation of this file](QfVariableEditor_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

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
    color: QfTheme.mainBackgroundColor
    border.color: QfTheme.controlBorderColor
    border.width: 1

    ListView {
      id: table
      flickableDirection: Flickable.VerticalFlick
      boundsBehavior: Flickable.StopAtBounds
      clip: true
      spacing: 1
      anchors.fill: parent

      ScrollBar.vertical: QfScrollBar {}

      model: QfExpressionVariableModel {
        currentProject: qgisProject
      }

      section.property: "VariableScope"
      section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
      section.delegate: Component {
        Rectangle {
          width: parent.width
          height: 30
          color: QfTheme.controlBorderColor

          Text {
            anchors {
              horizontalCenter: parent.horizontalCenter
              verticalCenter: parent.verticalCenter
            }
            font: QfTheme.strongResultFont
            color: QfTheme.mainTextColor
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
        property bool canDelete: VariableEditable && VariableScope == QfExpressionVariableModel.GlobalScope

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
              enabled: VariableScope == QfExpressionVariableModel.GlobalScope && VariableEditable
              font.bold: true
              font.pointSize: QfTheme.tinyFont.pointSize
              color: QfTheme.mainTextColor
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
                font: QfTheme.defaultFont
                horizontalAlignment: TextInput.AlignLeft
                placeholderText: !variableValueText.activeFocus && displayText === '' ? qsTr("Enter value") : ''
                color: QfTheme.mainTextColor

                onTextChanged: {
                  if (enabled && VariableValue != text) {
                    VariableValue = text;
                    if (VariableScope == QfExpressionVariableModel.ProjectScope) {
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

              iconSource: QfTheme.getThemeVectorIcon('ic_delete_forever_white_24dp')
              iconColor: QfTheme.mainTextColor
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
      let insertionPosition = table.model.addVariable(QfExpressionVariableModel.GlobalScope, "new_variable", "");
      table.positionViewAtIndex(insertionPosition, ListView.Contain);
      table.itemAtIndex(insertionPosition).forceFocusOnVariableName();
    }
  }
}
```


