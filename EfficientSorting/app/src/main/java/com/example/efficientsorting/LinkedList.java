package com.example.efficientsorting;

import androidx.annotation.NonNull;

public class LinkedList<E extends Comparable<E>> {
    private Node first;

    public LinkedList() {
    }

    public void add(E element) {
        this.first = new Node(element, this.first);
    }

    private Node middle(Node firstNode) {
        Node c = firstNode, m = firstNode;
        boolean t = false;
        while (c != null) {
            if (t) m = m.getNext();
            t = !t;
            c = c.getNext();
        }
        return m;
    }

    // Ordena la llista amb mergesort
    public void mergeSort() {
        mergeSort(this.first);
    }

    // mergeSort s’encarrega d’iniciar
    // l’ordenacio de la llista
    // Es crida dins mergeSort ()
    // i retorna el punter a l’inici de la llista ordenada
    // Adapta el mergeSort vist a classe
    private Node mergeSort(Node n) {
        return null;
    }

    // S’encarrega de fusionar les dues subllistes ordenades l, r
    // Retorna el punter a l’inici de la llista fusionada
    private Node merge(Node l, Node r) {
        return null;
    }

    @NonNull
    @Override
    public String toString() {
        Node node = this.first;
        StringBuilder str = new StringBuilder();
        for (int i = 0; i < 20 && node != null; i++) {
            str.append(node.getItem());
            node = node.getNext();
            if (node != null) str.append(", ");
        }
        return str.toString();
    }
}