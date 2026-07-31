set(QFIELD_THEME_QML_FILES
    QfActionButton.qml
    QfBadge.qml
    QfButton.qml
    QfCalendarItem.qml
    QfCalendarPanel.qml
    QfCollapsibleMessage.qml
    QfComboBox.qml
    QfContainerCard.qml
    QfDialog.qml
    QfDropShadow.qml
    QfExpandableGroupBox.qml
    QfMenu.qml
    QfMeterBar.qml
    QfOpacityMask.qml
    QfOverlayContainer.qml
    QfPageHeader.qml
    QfPaneDrawer.qml
    QfPopup.qml
    QfProgressRing.qml
    QfProjectThumbnail.qml
    QfScrollBar.qml
    QfSearchBar.qml
    QfSlider.qml
    QfSwipeAnimator.qml
    QfSwitch.qml
    QfTabBar.qml
    QfTextField.qml
    QfTimeItem.qml
    QfToggleButtonGroup.qml
    QfToolButton.qml
    QfToolButtonDrawer.qml
    QfToolButtonPie.qml
    QfVisibilityFadingRow.qml
    QfWelcomeAction.qml)

qt_add_library(qfield_qml_theme STATIC)

# Required by qt_add_qml_module, even without C++ sources here.
set_target_properties(qfield_qml_theme PROPERTIES AUTOMOC TRUE)

qt_add_qml_module(
  qfield_qml_theme
  URI
  Theme
  VERSION
  1.0
  NO_PLUGIN
  RESOURCE_PREFIX
  "/qt/qml"
  QML_FILES
  ${QFIELD_THEME_QML_FILES})

qt_import_qml_plugins(qfield_qml_theme)

# The import scanner needs absolute paths (QTBUG-80847).
set(QFIELD_THEME_QML_FILES_ABS "")
foreach(qml_file ${QFIELD_THEME_QML_FILES})
  list(APPEND QFIELD_THEME_QML_FILES_ABS
       "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()
set_property(TARGET qfield_qml_theme PROPERTY QML_FILES
                                              ${QFIELD_THEME_QML_FILES_ABS})
