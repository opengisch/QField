import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: popup

    property alias model: deltaList.model

    width: Math.min( 400, mainWindow.width - Theme.popupScreenEdgeMargin * 2 )
    x: (parent.width - width) / 2
    y: (parent.height - page.height) / 2
    padding: 0

    onOpened: function () {
      page.height = mainWindow.height - 160 + 60

      if ( cloudProjectsModel.currentProjectId ) {
        cloudProjectsModel.refreshProjectDeltaList(cloudProjectsModel.currentProjectId)
      }
    }

    onClosed: function () {
      deltaList.model = null
    }

    Page {
        id: page
        width: parent.width
        height: deltaList.height + 60
        padding: 10
        header: ToolBar {
          id: toolBar
          height: 48

          background: Rectangle {
            color: "transparent"
          }

          Label {
            anchors.centerIn: parent
            leftPadding: 48
            rightPadding: 48
            width: parent.width - 20
            text: !!model ? qsTr( "Push History" ) : qsTr( "Loading…" )
            font: Theme.strongFont
            color: Theme.mainColor
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }

          QfToolButton {
            id: closeButton
            anchors {
              top: parent.top
              right: parent.right
            }
            iconSource: Theme.getThemeIcon( 'ic_close_black_24dp' )
            bgcolor: "white"

            onClicked: {
              popup.close();
            }
          }
        }

        Column {
            spacing: 4
            width: parent.width

            ListView {
                id: deltaList
                width: parent.width
                height: mainWindow.height - 160
                clip: true

                delegate: Rectangle {
                    id: rectangle
                    width: parent ? parent.width : undefined
                    height: inner.height
                    color: "transparent"

                    ColumnLayout {
                        id: inner
                        width: parent.width

                        Text {
                            Layout.fillWidth: true
                            topPadding: 5
                            leftPadding: 5
                            text: {
                              var dt = new Date(CreatedAt)
                              return dt.toLocaleString()
                            }
                            font: Theme.defaultFont
                            color: "black"
                            wrapMode: Text.WordWrap
                        }

                        Text {
                            Layout.fillWidth: true
                            leftPadding: 5
                            bottomPadding: 5
                            text: {
                                var status = ''
                                switch(Status) {
                                  case DeltaListModel.PendingStatus:
                                    status = 'pending'
                                    break;
                                  case DeltaListModel.BusyStatus:
                                    status = 'busy'
                                    break;
                                  case DeltaListModel.AppliedStatus:
                                    status = 'applied'
                                    break;
                                  case DeltaListModel.ConflictStatus:
                                    status = 'conflict'
                                    break;
                                  case DeltaListModel.NotAppliedStatus:
                                    status = 'not applied'
                                    break;
                                  case DeltaListModel.ErrorStatus:
                                    status = 'error'
                                    break;
                                }
                                return 'Status: ' + status + ( Output != '' ? ' (' + Output + ')' : '' )
                              }
                            font: Theme.tipFont
                            color: Theme.gray
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }
}
