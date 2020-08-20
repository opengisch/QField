import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import Theme 1.0

Item {
  signal close()

  height: childrenRect.height
  width: parent.width

  GridLayout {
    id: mainGrid

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 20

    columns: 1

    Item {
        // top margin
        height: 20
    }

    Text {
      id: title
      text: qsTr( "What's new in the latest QField" )
      color: Theme.mainColor
      font: Theme.titleFont
      minimumPixelSize: 12


      fontSizeMode: Text.VerticalFit
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Text {
      id: changelogBody
      color: '#95000000'
      text: {
        var RELEASES_URL = 'https://api.github.com/repos/opengisch/qfield/releases'
        var xhr = new XMLHttpRequest()
        var parseVersion = function(str) {
          str = str.replace(/^[a-z]*/, '')

          var parts = str.split('.')

          if (parts[0] >= 0 && parts[1] >= 0 && parts[2] >= 0)
            return [parts[0], parts[1], parts[2]]

          return []
        }

        xhr.open('GET', RELEASES_URL)
        xhr.onreadystatechange = function() {
          if (xhr.readyState === XMLHttpRequest.DONE) {
            var resp = xhr.responseText
            var changelog = ''

            try {
              var releases = JSON.parse(resp)
              var qfieldVersion = parseVersion(version)

              for (var i = 0, l = releases.length; i < l; i++) {
                var release = releases[i]
                var releaseVersion = parseVersion(release['tag_name'])

                if (releaseVersion.length === 0)
                  continue

                var releaseChangelog = '\n#\n# ' + release['name'] + '\n\n' + release['body'] + '\n'

                // most probably developer version with no proper version set
                if (qfieldVersion.length === 0)
                  qfieldVersion = releaseVersion

                if (qfieldVersion[0] !== releaseVersion[0] || qfieldVersion[1] !== releaseVersion[1])
                  continue

                // prepend the current release
                changelog = releaseChangelog + changelog
              }

              if ( changelog.length === 0 )
                throw new Error('Empty changelog!')

              changelog += '\n' + '[' + qsTr('Previous releases on GitHub') + '](https://github.com/opengisch/qfield/releases)'
              changelog = changelog.replace(/^##(.+)$/gm, function(full) {
                return '\n###\n' + full + '\n\n\n'
              })

              changelogBody.text = changelog
            } catch (err) {
              changelogBody.text = qsTr('Temporarily cannot retrieve the changelog. Please check your internet connection.')
            }
          }
        }
        xhr.send()

        return qsTr('Loading…')
      }
      font: Theme.tipFont

      fontSizeMode: Text.VerticalFit
      textFormat: Text.MarkdownText
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.minimumHeight: contentHeight
      Layout.maximumHeight: contentHeight
    }

    Button {
      id: closeButton
      Layout.fillWidth: true
      Layout.fillHeight: true

      text: qsTr( 'OK' )

      font: Theme.defaultFont

      contentItem: Text {
        text: closeButton.text
        font: closeButton.font
        color: 'white'
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
      }

      background: Rectangle {
        color: Theme.mainColor
      }

      onClicked: {
        settings.setValue( '/QField/ChangelogVersion', versionCode )
        close()
      }
    }

    Item {
        // bottom
        height: 20
    }
  }
}
