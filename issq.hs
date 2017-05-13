{-# LANGUAGE NoImplicitPrelude, NoMonomorphismRestriction, FlexibleContexts #-}

import Data.List
import Data.Array -- .Unboxed
import Data.Bits
import Prelude
import Control.Exception
import Control.Parallel.Strategies
-- import Control.DeepSeq

-- qresall _N = nub $ map (\x -> x^2 `mod` _N) [0.._N-1]

-- memoqresall = (map qresall [0..] !!) . fromIntegral

-- aqresall _N = listArray (0, length xs - 1) xs
--   where xs = qresall _N

-- arr = listArray (3,65) $ map aqresall [3..65]
-- arrqresall = (!) arr . fromIntegral

-- -- qresnotsq :: Int -> [Int] -> Bool
-- qresnotsq sq ns = not $ all (\n -> any (\x -> sq `mod` n == x) $ elems $ arrqresall n) ns

qresall :: (Bits a, Integral a) => Int -> a
qresall n = foldl' (\set x -> setBit set $ x^2 `mod` n) 0 [0..n-1]


-- filters

-- arr :: (Integral e, Num i, Ix i, Bits e, IArray UArray e) => Array i e
arr = listArray (3,65) $ map qresall [3..65]
arrqresall :: (Ix a, Bits a, Integral a) => a -> a
arrqresall = (!) arr -- . fromIntegral

qresnotsq :: Int -> [Int] -> Bool
qresnotsq sq ns = not $ all (\n -> testBit (arrqresall n) (sq `mod` n)) ns

coverage fst lst ns = f fst 0
  where f i c
          | i > lst = c
          | True = f (i+1) $! if qresnotsq i ns then (c+1) else c
-- coverage fst lst ns = foldl' (\c i -> if qresnotsq i ns then (c+1) else c) 0 [fst..lst]

-- Given a list of `N's return the percentage of filtered squares
qresper ns = (fromIntegral $ coverage 0 lcm' ns) / (fromIntegral lcm')
  where lcm' = foldl1' lcm ns

set i x xs = take i xs ++ x : drop (i+1) xs

-- kCn :: Integral a => a -> [a] -> [[a]]
kCn k xs = next [0..k-1]
  where n = length xs
        next js = (map (xs!!) js) : if islast 0 js then [] else (next (nextjs js))
        nextjs js = inc (k-1) js
          where inc i js
                  | i == 0 && islast i js = js
                  | islast i js = reset i $ inc (i-1) js
                  | True = set i ((js!!i)+1) js
                reset i js = assert (i /= 0) $ set i ((js!!(i-1))+1) js
        islast i js = (js !! i) == (n-1) - (k-1 - i)


qresbest :: [(Double, [Int])]
qresbest = take 10 $ sortBy (flip compare) $ ((withStrategy (parList rdeepseq)) . map (\ns -> (qresper ns, ns))) lsts
  where lsts = concat $ map (\k -> kCn k [3..65]) [1..5]
        
main = print $ qresbest
