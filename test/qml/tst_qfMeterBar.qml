import QtQuick
import QtQuick.Controls
import QtTest
import org.qfield
import Theme

TestCase {
  name: "QfMeterBar"
  when: windowShown

  QfMeterBar {
    id: meterBar
    width: 300
    animationDuration: 0
  }

  property var progressBar: meterBar.children[0].children[1]

  function cleanup() {
    meterBar.value = 0;
  }

  function test_01_defaultProperties() {
    compare(meterBar.relatedUrl, "");
    compare(meterBar.usageText, "");
    compare(meterBar.warningThreshold, 0.8);
    compare(meterBar.criticalThreshold, 0.95);
  }

  function test_02_thresholdColors() {
    // Below warning -> normal color
    meterBar.value = 0.5;
    compare(progressBar.barColor, meterBar.normalColor);

    // At warning threshold -> warning color
    meterBar.value = 0.85;
    compare(progressBar.barColor, meterBar.warningColor);

    // At critical threshold -> critical color
    meterBar.value = 0.98;
    compare(progressBar.barColor, meterBar.criticalColor);
  }
}
