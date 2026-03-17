void delay_s(unsigned int d_s){
	for(d_s*=12000000; d_s>0; d_s--);
}

void delay_ms(unsigned int d_ms){
	for(d_ms*=12000; d_ms>0; d_ms--);
}

void delay_us(unsigned int d_us){
	for(d_us*=12; d_us>0; d_us--);
}
