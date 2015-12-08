(defproject lab6-mpi "0.1.0-SNAPSHOT"
  :description "MPI-based parallel matrix computations. MPJ is used as MPI implementation."
  :url "http://example.com/FIXME"
  :license {:name "The GNU General Public License v3.0"
            :url  "http://www.gnu.org/licenses/gpl-3.0.en.html"}
  :repositories {"local" "file:maven_repository"}
  :dependencies [[org.clojure/clojure "1.8.0-RC3"]
                 [net.mikera/core.matrix "0.47.0"]]
  :main ^:skip-aot lab6-mpi.core
  :target-path "target/%s"
  :profiles {:uberjar  {:aot :all}
             :provided {:dependencies [[tk.exarus.kpi.parallel.mpi/mpj "0.44"]
                                       [log4j "1.2.17"]]}})
