OptOn(OPTf_WARN_PAREN);


F64 d,d1=1,d2=1;

d1=(d2=3.0);

d1=(d2+3.0);

d=(d1+d2)+(d1+d2);

d=(2`3)`2;

d=2`(3`2);

d=-d1`(d+d2);


OptOff(OPTf_WARN_PAREN);
