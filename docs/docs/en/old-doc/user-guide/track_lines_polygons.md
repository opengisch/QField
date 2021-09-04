---
title: Tracking of lines and polygons
---

You have the possibility to do tracking on lines and polygons.

It\'s available when GPS is enabled. A tracking session can be started
over the legend - long press on the layer.

::: {.container .clearer .text-center}
![track\_lines\_started](../../assets/images/track_lines_started.jpeg)
:::

The time interval and the minimum distance can be entered individually
to define when a vertex needs to be set.

::: {.container .clearer .text-center}
![tracker\_settings](../../assets/images/track_lines_tracker_settings.jpeg)
:::

There can be several trackings session active on several layers. But
only one tracking session per layer can be active at the time.

::: {.container .clearer .text-center}
![tracking\_layers1](../../assets/images/track_lines_tracking_layers1.jpeg)

![tracking\_layers2](../../assets/images/track_lines_tracking_layers2.jpeg)
:::

The tracked feature is saved on every vertex. While the tracking session
is active the layer is still editable, but the tracked feature can\'t be
edited or deleted until the tracking session is stopped over the legend
again.

::: {.container .clearer .text-center}
![stop\_tracking\_delete](../../assets/images/track_lines_stop_tracking_delete.jpeg)
:::

If the layer geometry supports m value, the time is stored in the m
value, that passed since the first vertex of this tracking session has
been recorded.
