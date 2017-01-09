#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int sign_a = a >> 31;
	int sign_b = b >> 31;
	if(sign_a != 0) 
		a = -a;
	if(sign_b != 0)
		b = -b;
	unsigned int a1 = (a >> 16);
	unsigned int b1 = (b >> 16);  //high 16
	unsigned int a0 = a & 0xffff;
	unsigned int b0 = b & 0xffff;   //low 16
	unsigned int r0,r1,r2;
	r0 = a0 * b0;
	r1 = r0 / 0xffff + a0 * b1 + a1 * b0; 
	r0 %= 0xffff;
	r2 = r1 / 0xffff + a1 * b1; 
	r1 %= 0xffff;
	r2 %= 0xffff;
	int result = r1 + (r2 << 16);
	if(sign_a != sign_b) 
		result = -result;
	return result;
	
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/*unsigned int a00 = a << 16;
	unsigned int a01 = a >> 16;
	unsigned int a10 = a >> 31;
	unsigned int a11 = a >> 31;
	int ans = 0, i;
	for(i = 0; i < 64; ++i)
	{
		a11 = (a11 << 1) + (a10 >> 31);
		a10 = (a10 << 1) + (a01 >> 31);
		a01 = (a01 << 1) + (a00 >> 31);
		a00 = a00 << 1;
		ans = ans << 1;
		if(a11 > 0 || a10 >= b) {
			if(a10 < b) a11 --;
			a10 -= b;
			ans++; 
		}
	}
	return ans;*/
	int pos_a = a;
    	int pos_b = b;
	int result = 0;
	int front = 0;
	(a > 0) ? (pos_a = a) : (pos_a = -a);
	(b > 0) ? (pos_b = b) : (pos_b = -b);
	if(pos_a >= pos_b)
		front = pos_a / pos_b;
	result |= (front << 16);
	pos_a -=(pos_b * front);
	int i;
	for(i = 16;pos_a != 0 && i>= 0;i--)
	{
		if(pos_a < pos_b)
		{
			pos_a  <<=  1;
			continue;
		}
		else
		{
			result |= (1 << i);
			pos_a -= pos_b;
			pos_a  <<= 1;
		}
		
	}
	if(( a > 0 && b < 0 ) || ( a < 0 && b > 0 ))
		result = -result;
	return result;

}

FLOAT f2F(float a) {
	/*int i, uf, m, e, s, ans;
	uf = *(int*)&a;
	m = uf & ((1 << 23) - 1);
	e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
	s = uf >> 31;
	ans = 1;
	for(i = 1; i <= e + 16; ++ i) {
		ans = (ans << 1) + ((m & (1 << 22)) >> 22);
		if (ans < 0) return 0x80000000u;
		m = m << 1;
	}
	if (s != 0) ans = (~ans) + 1;
	return (FLOAT)(ans);*/
	
	int f = *(int *)&a;
	int m = f & 0x7fffff;
	int result = m | 0x800000;
	int exp = ((f >> 23) & 0xff) - 127;
	int shift = exp - 7;
	if(((f >> 23) & 0xff) == 0xff || shift > 7)
		return 0x80000000;
	if(shift >= 0)
	{
		if(f < 0)
			result = -(result << shift);
		else
			result = result << shift;	
	}
	else
	{
		if(f < 0)
			result = -(result >> (-shift));
		else
			result = result >> (-shift);
	}
	return result;
}

FLOAT Fabs(FLOAT a) {
	if(a>=0)
	   return a;
	else
	   return -a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
 	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
 	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

