package main

import "math/big"

func main() {
	m127 := big.NewInt(2)
	m127.Exp(m127, 127, nil)
	m127.Sub(1)

	mm127 := big.NewInt(0)
	mm127.SetBytes(m127.GetBytes())

	p := big.NewInt(0)

	for k := 1; k < 5; k++ {
		p.SetBytes(m127.GetBytes())
		p.Mul(2)
		p.Mul(k)
		p.Mul(m127)
		p.Add(1)
		r := big.NewInt(0)
		r.Exp(m127)
	}
}
