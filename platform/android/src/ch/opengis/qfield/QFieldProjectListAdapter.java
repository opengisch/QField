package ch.opengis.qfield;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import java.io.File;
import java.util.ArrayList;

public class QFieldProjectListAdapter
    extends ArrayAdapter<QFieldProjectListItem> {

    private final Context context;
    private final ArrayList<QFieldProjectListItem> values;

    static class ViewHolder {
        ImageView image;
        TextView text;
        ImageButton button;
    }

    public interface MenuButtonListener {
        void onClick(View view, int position);
    }
    MenuButtonListener menuButtonListener;

    public QFieldProjectListAdapter(Context context,
                                    ArrayList<QFieldProjectListItem> values,
                                    MenuButtonListener menuButtonListener) {

        super(context, android.R.layout.simple_list_item_2, android.R.id.text1,
              values);

        this.context = context;
        this.values = values;
        this.menuButtonListener = menuButtonListener;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        QFieldProjectListItem item = values.get(position);

        View rowView = convertView;
        if (rowView == null) {
            LayoutInflater inflater = (LayoutInflater)context.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
            ViewHolder h = new ViewHolder();

            if (item.getType() == QFieldProjectListItem.TYPE_SEPARATOR) {
                rowView = inflater.inflate(R.layout.list_separator, null);
                h.text = (TextView)rowView.findViewById(R.id.separator);
            } else {
                rowView =
                    inflater.inflate(R.layout.list_project_item, parent, false);
                h.image = (ImageView)rowView.findViewById(R.id.item_icon);
                h.text = (TextView)rowView.findViewById(R.id.item_title);
                h.button = (ImageButton)rowView.findViewById(R.id.item_menu);
            }
            rowView.setTag(h);
        }

        ViewHolder h = (ViewHolder)rowView.getTag();
        h.text.setText(item.getText());
        if (item.getType() != QFieldProjectListItem.TYPE_SEPARATOR) {
            h.image.setImageResource(item.getImageId());
            h.image.setImageAlpha(172);
            if (item.getType() == QFieldProjectListItem.TYPE_ITEM &&
                menuButtonListener != null) {
                h.button.setTag(position);
                h.button.setVisibility(View.VISIBLE);
                h.button.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        int position = (int)view.getTag();
                        menuButtonListener.onClick(view, position);
                    }
                });
            } else {
                h.button.setVisibility(View.GONE);
            }
        }

        return rowView;
    }
}
