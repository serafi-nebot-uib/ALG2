import com.sun.xml.internal.ws.addressing.WsaActionUtil;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Collections;
import java.util.PriorityQueue;

public class Main {
    public static void main(String[] args) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
            String str;
            while ((str = reader.readLine()) != null) {
                if (str.equals("0")) break;

                int h = Integer.parseInt(str);
                if ((str = reader.readLine()) == null) break;
                String[] arr = str.trim().split("\\s+");
                if (h != arr.length) break;

                PriorityQueue<Integer> heap = new PriorityQueue<>(h, Collections.reverseOrder());
                for (String s : arr) heap.offer(Integer.parseInt(s));

                if ((str = reader.readLine()) == null) break;
                int n = Integer.parseInt(str);
                if ((str = reader.readLine()) == null) break;
                arr = str.trim().split("\\s+");
                if (arr.length != n) break;

                boolean fit = true;
                for (int i = 0; i < arr.length && fit; i++) {
                    if (!heap.isEmpty()) {
                        int hangar = heap.poll();
                        int nave = Integer.parseInt(arr[i]);
                        int res = hangar - nave;
                        if (res > 0) heap.offer(res);
                        else if (res < 0) fit = false;
                    } else {
                        fit = false;
                    }
                }
                System.out.println(fit ? "SI" : "NO");
            }
        } catch (Exception ignored) {
//            throw new RuntimeException(ignored);
        }
    }
}