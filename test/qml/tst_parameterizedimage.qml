import QtQuick
import QtTest
import org.qfield

TestCase {
  name: "ParameterizedImage"
  when: windowShown

  ParameterizedImage {
    id: img
    width: 100
    height: 100
  }

  SignalSpy {
    id: sourceChangedSpy
    target: img
    signalName: "sourceChanged"
  }
  SignalSpy {
    id: fillColorChangedSpy
    target: img
    signalName: "fillColorChanged"
  }
  SignalSpy {
    id: strokeColorSpy
    target: img
    signalName: "strokeColorChanged"
  }
  SignalSpy {
    id: strokeWidthSpy
    target: img
    signalName: "strokeWidthChanged"
  }
  SignalSpy {
    id: parametersSpy
    target: img
    signalName: "parametersChanged"
  }

  function init() {
    img.source = "";
    img.fillColor = Qt.rgba(0, 0, 0, 1);
    img.strokeColor = Qt.rgba(1, 1, 1, 1);
    img.strokeWidth = 1.0;
    img.parameters = {};
    sourceChangedSpy.clear();
    fillColorChangedSpy.clear();
    strokeColorSpy.clear();
    strokeWidthSpy.clear();
    parametersSpy.clear();
  }

  function test_defaultState() {
    compare(img.source, "");
    compare(img.fillColor, Qt.rgba(0, 0, 0, 1));
    compare(img.strokeColor, Qt.rgba(1, 1, 1, 1));
    compare(img.strokeWidth, 1.0);
    compare(img.parameters, ({}));
  }

  function test_setSource_svg_emitsSourceChanged() {
    img.source = ":/themes/qfield/nodpi/ic_folder_qfield_param_48dp.svg";
    compare(sourceChangedSpy.count, 1);
  }

  function test_setSource_deduplication() {
    img.source = ":/themes/qfield/nodpi/ic_folder_qfield_param_48dp.svg";
    sourceChangedSpy.clear();
    img.source = ":/themes/qfield/nodpi/ic_folder_qfield_param_48dp.svg";
    compare(sourceChangedSpy.count, 0);
  }

  function test_setSource_qrcPrefix_stripped() {
    // "qrc:" prefix must be stripped internally; both spellings refer to the same source
    img.source = "qrc:/themes/qfield/nodpi/ic_folder_qfield_param_48dp.svg";
    compare(sourceChangedSpy.count, 1);
    var sourceWithoutPrefix = img.source;
    img.source = "";
    sourceChangedSpy.clear();
    img.source = ":/themes/qfield/nodpi/ic_folder_qfield_param_48dp.svg";
    compare(img.source, sourceWithoutPrefix);
  }

  function test_setSource_raster_emitsSourceChanged() {
    img.source = ":/pictures/qfield-love.png";
    compare(sourceChangedSpy.count, 1);
  }

  function test_setFillColor_emitsSignal() {
    img.fillColor = Qt.rgba(1, 0, 0, 1);
    compare(fillColorChangedSpy.count, 1);
    compare(img.fillColor, Qt.rgba(1, 0, 0, 1));
  }

  function test_setFillColor_deduplication() {
    img.fillColor = Qt.rgba(0, 0, 0, 1);
    compare(fillColorChangedSpy.count, 0);
  }

  function test_setStrokeColor_emitsSignal() {
    img.strokeColor = Qt.rgba(0, 1, 0, 1);
    compare(strokeColorSpy.count, 1);
    compare(img.strokeColor, Qt.rgba(0, 1, 0, 1));
  }

  function test_setStrokeColor_deduplication() {
    img.strokeColor = Qt.rgba(1, 1, 1, 1);
    compare(strokeColorSpy.count, 0);
  }

  function test_setStrokeWidth_emitsSignal() {
    img.strokeWidth = 3.0;
    compare(strokeWidthSpy.count, 1);
    compare(img.strokeWidth, 3.0);
  }

  function test_setStrokeWidth_deduplication() {
    img.strokeWidth = 1.0;
    compare(strokeWidthSpy.count, 0);
  }

  function test_setParameters_emitsSignal() {
    img.parameters = {
      "color": "#ff0000",
      "size": "48"
    };
    compare(parametersSpy.count, 1);
  }

  function test_setParameters_deduplication() {
    img.parameters = {};
    compare(parametersSpy.count, 0);
  }

  function test_setParameters_nonStringValuesIgnored() {
    // only string-convertible values are kept in mPreparedParameters
    let params = {
      "valid": "yes"
    };
    params["invalid"] = null;
    img.parameters = params;
    compare(parametersSpy.count, 1);
  }

  function test_rasterSource_colorChangeStillEmitsSignal() {
    // color signals must still fire even though update() is skipped for rasters
    img.source = ":/pictures/qfield-love.png";
    fillColorChangedSpy.clear();
    img.fillColor = Qt.rgba(0, 0, 1, 1);
    compare(fillColorChangedSpy.count, 1);
  }
}
