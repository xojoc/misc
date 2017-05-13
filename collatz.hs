import Data.Ratio

data Collatz a = Half (Collatz a) | Triple (Collatz a) | Init a
		deriving (Show)

eval :: Integral a => Collatz a -> Ratio a
eval (Init x) = x % 1
eval (Half x) = (eval x) / 2
eval (Triple x) = (eval x) * 3 + 1

findSol :: Integral a => Collatz a -> (Ratio a, Ratio a) -> (Ratio a, Ratio a)
findSol (Init x) (coef, const) = (coef, const)
findSol (Half x) (coef, const) = findSol x (coef * 2, const * 2)
findSol (Triple x) (coef, const) = findSol x (coef / 3, (const-1)/3)

findx (Init x) = x%1
findx col = c / (1 - x)
	where (x,c) = findSol col (1%1, 0%1)

findCycle :: Integral a => a -> Collatz a -> [(Collatz a, Ratio a)]
findCycle 20 _ = []
findCycle n col@(Init x) = (col,x%1) : (findCycle (n+1) (Half col)) ++ (findCycle (n+1) (Triple col))
findCycle n col@(Half x) = (col,findx col) : (findCycle (n+1) (Half col)) ++ (findCycle (n+1) (Triple col))
findCycle n col@(Triple x) = (col,findx col) : (findCycle (n+1) (Half col))

