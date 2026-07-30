import QtQuick
import org.qfield

Item {
  signal prepareResult(var details)
  signal fetchResultsEnded

  function fetchResults(string, context, parameters) {
    prepareResult({
      "userData": {
        "term": string
      },
      "displayString": "echo: " + string,
      "description": "Plugin echo result",
      "score": 1.0,
      "group": "Plugin"
    });
    fetchResultsEnded();
  }
}
