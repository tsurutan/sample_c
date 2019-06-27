double x = x + x
quadruple x = double (double x)
n = a `div` length xs
    where
      a = 10
      xs = [1, 2, 3, 4, 5]
tlast xs = xs!!(length xs - 1)
trunc xs = take (length xs - 1) xs
trunc2 xs = reverse (tail (reverse xs))
