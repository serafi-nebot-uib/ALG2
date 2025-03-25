import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Collections;

// time complexity of PriorityQueue's methods:
//   offer(): O(log n)
//   peek(): O(1)
//   poll(): O(1)

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int h;
        // iterate until hangar size, h, is equal to 0
        // O(n log n) + O(n) + O(n log n) = O(n log n)
        while ((h = scanner.nextInt()) > 0) {
            // create a priority queue with a maximum capacity of h elements and reverse the order with
            // Collections.reverseOrder() so that elements are ordered in descending order
            PriorityQueue<Integer> heap = new PriorityQueue<>(h, Collections.reverseOrder());
            // O(n) * O(log n) = O(n log n)
            for (int i = 0; i < h; i++) // O(n)
                heap.offer(scanner.nextInt()); // O(log n)

            // read all spaceships and store them in an array in the order of arrival
            int n = scanner.nextInt();
            int[] spaceships = new int[n];
            // O(n)
            for (int i = 0; i < n; i++) spaceships[i] = scanner.nextInt();

            boolean fit = true; // indicates whether all spaceships fit into the hangars following the assignment method
            // iterate over all spaceships while `fit` is still true
            // O(n) * (O(1) + O(log n)) = O(n) + O(n log n) = O(n log n)
            for (int i = 0; i < n && fit; i++) { // O(n)
                // when there are no hangars left or
                //   the current spaceship cannot fit in the largest hangar (the first element in the queue)
                //   the assignment method does not work for this case -> fit = false
                if (heap.isEmpty() || heap.peek() < spaceships[i]) fit = false; // O(1)
                // when the current spaceship fits into the largest hangar -> remove the first hangar and
                //   add it back to the queue with the new size
                else heap.offer(heap.poll() - spaceships[i]); // O(log n) + O(1) = O(log n)
            }
            System.out.println(fit ? "SI" : "NO");
        }

        scanner.close();
    }
}