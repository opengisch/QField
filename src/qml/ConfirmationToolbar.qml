import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

VisibilityFadingRow {
    id: confirmationToolbar

    spacing: 4

    /* This signal is emitted when the digitized geometry has been confirmed.
     * The correspoding handler is \c onConfirmed.
     */
    signal confirm
    /* This signal is emitted when the user cancels geometry digitizing.
     * The correspoding handler is \c onCancel.
     */
    signal cancel

    QfToolButton {
        id: cancelButton
        iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
        visible: true
        round: true
        bgcolor: Theme.darkRed

        onClicked: cancel()
    }

    QfToolButton {
        id: confirmButton
        iconSource: Theme.getThemeIcon( "ic_check_white_48dp" )
        visible: true
        round: true
        bgcolor: Theme.mainColor

        onClicked: confirm()
    }
}
