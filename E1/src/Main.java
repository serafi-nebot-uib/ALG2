import java.util.PriorityQueue;
import java.util.Scanner;

class Hangar implements Comparable<Hangar> {
    private int capacity;

    public Hangar(int capacity) {
        this.capacity = capacity;
    }

    public int getCapacity() {
        return capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
    }

    @Override
    public int compareTo(Hangar o) {
        return o.capacity - this.capacity;
    }
}

// time complexity of PriorityQueue's methods:
//   offer(): O(log n)
//   peek(): O(1)
//   poll(): O(1)

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // iterate until hangar size, h, is equal to 0
        // O(n log n) + O(n log n) = O(n log n)
        for (int h = scanner.nextInt(); h != 0; h = scanner.nextInt()) {
            // create a priority queue with a maximum capacity of h elements and reverse the order with
            // Collections.reverseOrder() so that elements are ordered in descending order
            PriorityQueue<Hangar> heap = new PriorityQueue<>(h);
            // O(n) * O(log n) = O(n log n)
            for (int i = 0; i < h; i++) // O(n)
                heap.offer(new Hangar(scanner.nextInt())); // O(log n)

            int n = scanner.nextInt(); // number of spaceships
            boolean fit = true; // indicates whether all spaceships fit into the hangars following the assignment method
            // iterate over all spaceships while `fit` is still true
            // O(n) * (O(1) + O(log n)) = O(n) + O(n log n) = O(n log n)
            for (int i = 0; i < n; i++) { // O(n)
                int spaceship = scanner.nextInt();
                if (!fit) continue;
                Hangar hangar = heap.peek(); // O(1)
                // hangar = null when heap is empty -> no need to call heap.isEmpty()
                if (hangar == null || hangar.getCapacity() < spaceship) { // O(1)
                    // when there are no hangars left or
                    //   the current spaceship cannot fit in the largest hangar (the first element in the queue)
                    //   the assignment method does not work for this case -> fit = false
                    fit = false;
                } else { // O(1) + O(log n) = O(log n)
                    // when the current spaceship fits into the largest hangar -> remove the first hangar and
                    //   add it back to the queue with the new size
                    heap.poll(); // O(1)
                    hangar.setCapacity(hangar.getCapacity() - spaceship);
                    if (hangar.getCapacity() > 0) heap.offer(hangar); // O(log n)
                }
            }
            System.out.println(fit ? "SI" : "NO");
        }

        scanner.close();
    }
}