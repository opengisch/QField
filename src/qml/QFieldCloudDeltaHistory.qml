import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: popup

    property alias model: deltasList.model

    width: Math.min( 400, mainWindow.width - 20 )
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    Page {
        id: page
        width: parent.width
        height: deltasList.height
        padding: 10
        header: Label {
            padding: 10
            topPadding: 15
            bottomPadding: 0
            width: parent.width - 20
            text: qsTr( "Push History" )
            font: Theme.strongFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Column {
            spacing: 4
            width: parent.width

            ListView {
                id: deltasList
                width: parent.width
                height: Math.min( childrenRect.height, mainWindow.height - 80 );
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
