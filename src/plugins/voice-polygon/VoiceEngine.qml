// src/plugins/voice-polygon/VoiceEngine.qml
import QtQuick
import QtSpeech
import org.qfield

Item {
  id: voiceEngine

  signal commandReceived(string rawText)
  signal errorOccurred(string message)
  signal listeningChanged(bool active)
  signal offlineUnavailable()

  property bool   isListening: false
  property string locale: "es-MX"

  Loader {
    id: androidBridgeLoader
    active: Qt.platform.os === "android"
    sourceComponent: androidBridgeComponent
  }

  Component {
    id: androidBridgeComponent
    AndroidVoiceBridge {
      id: androidBridge
      locale: voiceEngine.locale

      onResult: function(text) {
        voiceEngine.commandReceived(text.toLowerCase())
        voiceEngine.isListening = false
        voiceEngine.listeningChanged(false)
      }

      onOfflineModelMissing: {
        voiceEngine.isListening = false
        voiceEngine.listeningChanged(false)
        voiceEngine.offlineUnavailable()
      }

      onError: function(msg) {
        voiceEngine.isListening = false
        voiceEngine.listeningChanged(false)
        voiceEngine.errorOccurred("Android voz: " + msg)
      }
    }
  }

  SpeechRecognition {
    id: qtRecognition
    active: false
    locale: voiceEngine.locale
    onDeviceRecognition: true

    onResultReady: function(result) {
      if (result.isFinal) {
        voiceEngine.commandReceived(result.transcript.toLowerCase())
      }
    }

    onError: function(error) {
      const isOfflineError = (error === SpeechRecognition.Error.NotAvailable
                           || error === SpeechRecognition.Error.Network)
      if (isOfflineError) {
        voiceEngine.offlineUnavailable()
      } else {
        voiceEngine.errorOccurred("Error de voz: " + error)
      }
      voiceEngine.isListening = false
      voiceEngine.listeningChanged(false)
    }

    onActiveChanged: {
      voiceEngine.isListening = qtRecognition.active
      voiceEngine.listeningChanged(qtRecognition.active)
    }
  }

  function startListening() {
    if (Qt.platform.os === "android") {
      if (androidBridgeLoader.item) {
        voiceEngine.isListening = true
        voiceEngine.listeningChanged(true)
        androidBridgeLoader.item.startListening()
      }
    } else {
      qtRecognition.start()
    }
  }

  function stopListening() {
    if (Qt.platform.os === "android") {
      if (androidBridgeLoader.item) androidBridgeLoader.item.stopListening()
    } else {
      qtRecognition.stop()
    }
    voiceEngine.isListening = false
    voiceEngine.listeningChanged(false)
  }
}
