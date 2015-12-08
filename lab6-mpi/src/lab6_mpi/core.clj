; Паралельне прогорамування - 1. Основи паралельного програмування
; Лабораторна робота N 7.
; ПОТОКИ В БИБЛИОТЕКЕ MPI
; F1  1.16: d = ((A + B)* (C *(MA*MD)))
; F2  2.23: f = MAX(MK * ML - MO)
; F3  3.28: V = MAX(MR*S) + MIN((MT*MW + MV))
; Гунавардана Руслан ІП-31
; 6 грудня 2015 року н.э.

(ns lab6-mpi.core
  (:require [clojure.core.matrix :refer :all])
  (:import (mpi MPI))
  (:gen-class))

(defn -main
  "Parallel matrix expression calculations. Implemented using MPI library."
  [& args]
  (let [real-args (MPI/Init (into-array String args))
        n (if (empty? real-args) 4 (Integer/parseUnsignedInt (first real-args)))
        rank (.Rank MPI/COMM_WORLD)
        result (case rank
                 0 (let [A  (broadcast 1.0 [n])
                         B  (broadcast 1.0 [n])
                         C  (broadcast 1.0 [n])
                         MA (broadcast 1.0 [n n])
                         MD (broadcast 1.0 [n n])]
                     (dot (add A B) (mmul C (mmul MA MD))))
                 1 (let [MK (broadcast 1.0 [n n])
                         ML (broadcast 1.0 [n n])
                         MO (broadcast 1.0 [n n])]
                     (emax (sub (mmul MK ML) MO)))
                 2 (let [S  (broadcast 1.0 [n])
                         MR (broadcast 1.0 [n n])
                         MT (broadcast 1.0 [n n])
                         MW (broadcast 1.0 [n n])
                         MV (broadcast 1.0 [n n])]
                     (+ (emax (mmul MR S)) (emax (add (mmul MT MW) MV)))))]
    (println (str "Process (rank=" rank ") started. Total: " (.Size MPI/COMM_WORLD)))
    (println (str "Process (rank=" rank ") finished with result: " result)))
  (MPI/Finalize))