import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

RelationWidget {
  id: valueRelation
  _currentLayerId: config['Layer']
  _keyField: config['Key']
}
