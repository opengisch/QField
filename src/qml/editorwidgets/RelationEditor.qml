import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Frame{
    height: 100
    width: 400

    ListView {
        implicitHeight: 400
        implicitWidth: 100
        clip: true
        model: relationEditorModel
        delegate: RowLayout {
            focus: true
            width: parent.width
            TextField { text: model.displayString }
        }
    }
}

