(let [n 4
      B (broadcast 1. [n])
      C (broadcast 1. [n])
      MO (broadcast 1. [n n])
      MT (broadcast 1. [n n])
      ME (broadcast 1. [n n])
      MU (broadcast 1. [n n])]
     (add (mmul (mmul B MO) MT) (mmul C (mmul ME MU))))