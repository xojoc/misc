
import Data.List
import Data.Bits

--data Filter = Bits Integer

--toIntegerF = 

qres :: (Integral a, Bits b, Integral b) => Int -> (Int,b)
qres n = (,) n $ foldl' (\q x -> setBit q $ x^2 `mod` n) 0 [0..n-1]

appendF :: (Bits a, Integral a) => (Int, a) -> (Int,a) -> (Int,a)
appendF (n1, q1) (n2, q2) = (,) (n1+n2) $ shift q1 n2 .|. q2

replicateF :: (Bits a, Integral a, Enum b, Num b) => (Int,a) -> b -> (Int,a)
replicateF f@(n,q) k = foldl' (\f' _ -> appendF f' f) f [1..k-1] 


combineF :: (Bits a, Integral a) => (Int,a) -> (Int,a) -> (Int,a)
combineF f1@(n1,q1) f2@(n2,q2) = (l, q1' .|. q2')
	where
		l = lcm n1 n2
		(_,q1') = replicateF f1 (l `div` n1)
		(_,q2') = replicateF f2 (l `div` n2)


-- issqF f s = 