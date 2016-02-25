lein uberjar
IF "%1" NEQ "cluster" (
  mpjrun -np 3 -jar target\uberjar\lab6-mpi-0.1.0-SNAPSHOT-standalone.jar 3000
) ELSE (
 C:
 cd C:\Users\Public
 mpjdaemon -boot
 mpjrun -np 3 -dev niodev -src -jar target\uberjar\lab6-mpi-0.1.0-SNAPSHOT-standalone.jar 3000
)

