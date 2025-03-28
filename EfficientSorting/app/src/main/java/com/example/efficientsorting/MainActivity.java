package com.example.efficientsorting;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.android.material.textfield.TextInputEditText;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "EfficientSorting";
    private static SurfaceView drawing = null;
    public static CheckBox drawCheckBox = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        drawCheckBox = findViewById(R.id.checkBox);
        drawing = findViewById(R.id.surfaceView);
        Button runButton = findViewById(R.id.runButton);
        TextView randomTextView = findViewById(R.id.randomTextView);
        TextView linkedListTextView = findViewById(R.id.linkedListTextView);
        TextView orderedTextView = findViewById(R.id.orderedTextView);
        TextView totalTimeTextView = findViewById(R.id.totalTimeTextView);
        EditText lengthInput = findViewById(R.id.lengthTextInput);
        runButton.setOnClickListener((v) -> {
            Log.d(TAG, "button clicked");
            int n = Integer.parseInt(lengthInput.getText().toString());
            Log.d(TAG, String.format("array length: %d", n));
            int[] arr = Sorting.generateRandomArray(n);
            LinkedList<Integer> list = new LinkedList<>();
            for (int i = arr.length-1; i >= 0; i--) list.add(arr[i]);
            randomTextView.setText(String.format("Random array: %s", Sorting.printArray(arr)));
            linkedListTextView.setText(String.format("Linked list: %s", list));

            Thread thread = new Thread() {
                @Override
                public void run() {
                    runOnUiThread(() -> {
                        orderedTextView.setText("Ordered array: ");
                        totalTimeTextView.setText("Total time (ms): ");
                    });
                    long start = System.currentTimeMillis();
//                    Sorting.selectionSort(arr);
//                    Sorting.quickSort(arr, 0, arr.length-1);
                    Sorting.mergeSort(arr, 0, arr.length-1);
                    long time = System.currentTimeMillis() - start;
                    runOnUiThread(() -> {
                        orderedTextView.setText(String.format("Ordered array: %s", Sorting.printArray(arr)));
                        totalTimeTextView.setText(String.format("Total time (ms): %d", time));
                    });
                }
            };
            thread.start();
        });
    }

    public static void repaint(int[] arr) {
        if (drawing == null || drawCheckBox == null) return;
        if (!drawCheckBox.isChecked()) return;
        if (drawing.getHolder().getSurface().isValid()) {
            int height = drawing.getHeight();
            int width = drawing.getWidth();
            Canvas canvas = drawing.getHolder().lockCanvas();
            Paint paint = new Paint();
            paint.setColor(Color.BLACK);
            paint.setStyle(Paint.Style.FILL);

            canvas.drawRect(new Rect(0, 0, width, height), paint);

            paint.setColor(Color.YELLOW);
            float barWidth = width / (float) arr.length;
            int barHeight = height / Sorting.MAX_VAL;
            for (int i = 0; i < arr.length; i++) {
                float l = i*barWidth;
                canvas.drawRect(new Rect((int)l, height-arr[i]*barHeight, (int)(l+barWidth), height), paint);
            }

            drawing.getHolder().unlockCanvasAndPost(canvas);
        }
    }
}