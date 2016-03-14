; OmegaWalk: walks the full, infinite, omega-ary tree
; Copyright Ben Wiederhake 2016

; This is free and unencumbered software released into the public domain.
; For more information, please refer to the file 'LICENSE' file,
; or visit <http://unlicense.org>

; Language: Clojure
; Basic idea: lazy-seq
; How to run: Use some IDE like LightTable.
;    Sorry, but setting up leiningen is too intrusive.

(def omega-walk
  (cons
   '()
   ((fn comp-walk [i n]
      (let [[new-i new-n] (if (>= i n) [0 (inc n)] [(inc i) n])]
        (cons (lazy-seq (cons i (nth omega-walk (- n i))))
              (lazy-seq (comp-walk new-i new-n)))))
    0 0)))

(defn timed-print []
  ; 900 msecs on my machine
  (time (print (take 10000 omega-walk4))))
