package ch.opengis.qfield;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.pdf.PdfRenderer;
import android.net.Uri;
import android.os.ParcelFileDescriptor;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import java.io.File;
import java.net.URI;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Map;
import java.util.WeakHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class QFieldProjectListAdapter
    extends ArrayAdapter<QFieldProjectListItem> {

    private final Context context;
    private final ArrayList<QFieldProjectListItem> values;

    private Map<ImageView, String> imageViews =
        Collections.synchronizedMap(new WeakHashMap<ImageView, String>());
    ExecutorService executorService = Executors.newFixedThreadPool(2);

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
            if (item.getType() == QFieldProjectListItem.TYPE_ITEM) {
                if (item.getFile().isFile() &&
                    item.getFile().length() < 50000000) {
                    String fileName = item.getFile().getName();
                    String extension =
                        fileName.substring(fileName.lastIndexOf("."))
                            .toLowerCase();
                    if (extension.equals(".png") || extension.equals(".jpg") ||
                        extension.equals(".jpeg") ||
                        extension.equals(".webp") || extension.equals(".pdf")) {
                        String imagePath = item.getFile().getAbsolutePath();
                        imageViews.put(h.image, imagePath);
                        executorService.submit(
                            new ImageLoader(imagePath, h.image));
                    }
                }
                if (menuButtonListener != null) {
                    h.button.setTag(position);
                    h.button.setVisibility(View.VISIBLE);
                    h.button.setImageAlpha(172);
                    h.button.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            int position = (int)view.getTag();
                            menuButtonListener.onClick(view, position);
                        }
                    });
                }
            } else {
                h.image.setImageResource(item.getImageId());
                h.button.setVisibility(View.GONE);
            }
        }

        return rowView;
    }

    class ImageLoader implements Runnable {
        String imagePath;
        ImageView imageView;

        ImageLoader(String imagePath, ImageView imageView) {
            this.imagePath = imagePath;
            this.imageView = imageView;
        }

        @Override
        public void run() {
            if (imageViewReused(imagePath, imageView))
                return;

            String extension =
                imagePath.substring(imagePath.lastIndexOf(".")).toLowerCase();
            Bitmap bmp;
            try {
                if (extension.equals(".pdf")) {
                    PdfRenderer renderer =
                        new PdfRenderer(ParcelFileDescriptor.open(
                            new File(imagePath),
                            ParcelFileDescriptor.MODE_READ_ONLY));
                    PdfRenderer.Page page = renderer.openPage(0);

                    bmp = Bitmap.createBitmap(page.getWidth(), page.getHeight(),
                                              Bitmap.Config.ARGB_8888);
                    page.render(bmp, null, null,
                                PdfRenderer.Page.RENDER_MODE_FOR_DISPLAY);

                    page.close();
                    renderer.close();
                } else {
                    bmp = BitmapFactory.decodeFile(imagePath);
                }
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }

            if (imageViewReused(imagePath, imageView))
                return;
            BitmapDisplayer bd = new BitmapDisplayer(bmp, imagePath, imageView);
            Activity a = (Activity)imageView.getContext();
            a.runOnUiThread(bd);
        }
    }

    boolean imageViewReused(String imagePath, ImageView imageView) {
        String tag = imageViews.get(imageView);
        if (tag == null || !tag.equals(imagePath))
            return true;
        return false;
    }

    class BitmapDisplayer implements Runnable {
        Bitmap bitmap;
        String imagePath;
        ImageView imageView;

        public BitmapDisplayer(Bitmap bitmap, String imagePath,
                               ImageView imageView) {
            this.bitmap = bitmap;
            this.imagePath = imagePath;
            this.imageView = imageView;
        }

        public void run() {
            if (imageViewReused(imagePath, imageView))
                return;

            if (bitmap != null) {
                imageView.setImageBitmap(bitmap);
            }
        }
    }
}
