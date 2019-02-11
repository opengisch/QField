package ch.opengis.qfield;
 
import java.util.ArrayList;
import java.io.File;
 
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class QFieldProjectListAdapter extends ArrayAdapter {
 
        private final Context context;
        private final ArrayList values;
 
        public QFieldProjectListAdapter(Context context, ArrayList values) {
 
            super(context, android.R.layout.simple_list_item_2, android.R.id.text1, values);
 
            this.context = context;
            this.values = values;
        }
 
        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
 
            LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
 
            View rowView = null;

            File file = (File) values.get(position);
            
            rowView = inflater.inflate(R.layout.list_project_item, parent, false);
 
            ImageView imgView = (ImageView) rowView.findViewById(R.id.item_icon); 
            TextView titleView = (TextView) rowView.findViewById(R.id.item_title);

            if (file.isDirectory()){
                imgView.setImageResource(R.drawable.directory);                
            } else {
                imgView.setImageResource(R.drawable.icon);
            }
            
            titleView.setText(file.getName());
            
            return rowView;
        }
}
