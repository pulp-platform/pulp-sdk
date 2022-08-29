APP_CFLAGS=-O3 -g -DSKIP_PLL_INIT
riscv32-unknown-elf-objdump -Mmarch=rv32imc_zfinx_xpulpv3 -d /scratch/prasadar/GVSOC/test_dory_opensource/pulp-sdk/tests/manuel_test/BUILD/SIRACUSA/GCC_RISCV//test/test

/scratch/prasadar/GVSOC/test_dory_opensource/pulp-sdk/tests/manuel_test/BUILD/SIRACUSA/GCC_RISCV//test/test:     file format elf32-littleriscv


Disassembly of section .vectors:

1c008000 <__irq_vector_base>:
1c008000:	08c0006f          	j	1c00808c <pos_illegal_instr>
1c008004:	08c0006f          	j	1c008090 <pos_no_irq_handler>
1c008008:	0880006f          	j	1c008090 <pos_no_irq_handler>
1c00800c:	0840006f          	j	1c008090 <pos_no_irq_handler>
1c008010:	0800006f          	j	1c008090 <pos_no_irq_handler>
1c008014:	07c0006f          	j	1c008090 <pos_no_irq_handler>
1c008018:	0780006f          	j	1c008090 <pos_no_irq_handler>
1c00801c:	0740006f          	j	1c008090 <pos_no_irq_handler>
1c008020:	0700006f          	j	1c008090 <pos_no_irq_handler>
1c008024:	06c0006f          	j	1c008090 <pos_no_irq_handler>
1c008028:	0680006f          	j	1c008090 <pos_no_irq_handler>
1c00802c:	0640006f          	j	1c008090 <pos_no_irq_handler>
1c008030:	0600006f          	j	1c008090 <pos_no_irq_handler>
1c008034:	05c0006f          	j	1c008090 <pos_no_irq_handler>
1c008038:	0580006f          	j	1c008090 <pos_no_irq_handler>
1c00803c:	0540006f          	j	1c008090 <pos_no_irq_handler>
1c008040:	0500006f          	j	1c008090 <pos_no_irq_handler>
1c008044:	04c0006f          	j	1c008090 <pos_no_irq_handler>
1c008048:	0480006f          	j	1c008090 <pos_no_irq_handler>
1c00804c:	0440006f          	j	1c008090 <pos_no_irq_handler>
1c008050:	0400006f          	j	1c008090 <pos_no_irq_handler>
1c008054:	03c0006f          	j	1c008090 <pos_no_irq_handler>
1c008058:	0380006f          	j	1c008090 <pos_no_irq_handler>
1c00805c:	0340006f          	j	1c008090 <pos_no_irq_handler>
1c008060:	0300006f          	j	1c008090 <pos_no_irq_handler>
1c008064:	02c0006f          	j	1c008090 <pos_no_irq_handler>
1c008068:	0280006f          	j	1c008090 <pos_no_irq_handler>
1c00806c:	0240006f          	j	1c008090 <pos_no_irq_handler>
1c008070:	0200006f          	j	1c008090 <pos_no_irq_handler>
1c008074:	01c0006f          	j	1c008090 <pos_no_irq_handler>
1c008078:	0180006f          	j	1c008090 <pos_no_irq_handler>
1c00807c:	0140006f          	j	1c008090 <pos_no_irq_handler>

1c008080 <_start>:
1c008080:	00003517          	auipc	a0,0x3
1c008084:	2b850513          	addi	a0,a0,696 # 1c00b338 <pos_init_entry>
1c008088:	00050067          	jr	a0

1c00808c <pos_illegal_instr>:
1c00808c:	0000006f          	j	1c00808c <pos_illegal_instr>

1c008090 <pos_no_irq_handler>:
1c008090:	30200073          	mret

1c008094 <pos_semihosting_call>:
1c008094:	00100073          	ebreak
1c008098:	00008067          	ret

Disassembly of section .text:

1c00809c <__udivdi3>:
1c00809c:	88aa                	mv	a7,a0
1c00809e:	872e                	mv	a4,a1
1c0080a0:	87b2                	mv	a5,a2
1c0080a2:	8336                	mv	t1,a3
1c0080a4:	882a                	mv	a6,a0
1c0080a6:	1e069c63          	bnez	a3,1c00829e <__udivdi3+0x202>
1c0080aa:	1c001537          	lui	a0,0x1c001
1c0080ae:	86850513          	addi	a0,a0,-1944 # 1c000868 <__clz_tab>
1c0080b2:	0ac5fd63          	bgeu	a1,a2,1c00816c <__udivdi3+0xd0>
1c0080b6:	66c1                	lui	a3,0x10
1c0080b8:	0ad67363          	bgeu	a2,a3,1c00815e <__udivdi3+0xc2>
1c0080bc:	0ff00693          	li	a3,255
1c0080c0:	00c6b6b3          	sltu	a3,a3,a2
1c0080c4:	068e                	slli	a3,a3,0x3
1c0080c6:	00d65333          	srl	t1,a2,a3
1c0080ca:	951a                	add	a0,a0,t1
1c0080cc:	00054503          	lbu	a0,0(a0)
1c0080d0:	02000313          	li	t1,32
1c0080d4:	96aa                	add	a3,a3,a0
1c0080d6:	40d30533          	sub	a0,t1,a3
1c0080da:	00d30c63          	beq	t1,a3,1c0080f2 <__udivdi3+0x56>
1c0080de:	00a71733          	sll	a4,a4,a0
1c0080e2:	00d8d6b3          	srl	a3,a7,a3
1c0080e6:	00a617b3          	sll	a5,a2,a0
1c0080ea:	00e6e5b3          	or	a1,a3,a4
1c0080ee:	00a89833          	sll	a6,a7,a0
1c0080f2:	0107d313          	srli	t1,a5,0x10
1c0080f6:	0265d733          	divu	a4,a1,t1
1c0080fa:	01079893          	slli	a7,a5,0x10
1c0080fe:	0108d893          	srli	a7,a7,0x10
1c008102:	01085693          	srli	a3,a6,0x10
1c008106:	0265f633          	remu	a2,a1,t1
1c00810a:	853a                	mv	a0,a4
1c00810c:	02e885b3          	mul	a1,a7,a4
1c008110:	0642                	slli	a2,a2,0x10
1c008112:	8ed1                	or	a3,a3,a2
1c008114:	00b6fc63          	bgeu	a3,a1,1c00812c <__udivdi3+0x90>
1c008118:	96be                	add	a3,a3,a5
1c00811a:	fff70513          	addi	a0,a4,-1
1c00811e:	00f6e763          	bltu	a3,a5,1c00812c <__udivdi3+0x90>
1c008122:	00b6f563          	bgeu	a3,a1,1c00812c <__udivdi3+0x90>
1c008126:	ffe70513          	addi	a0,a4,-2
1c00812a:	96be                	add	a3,a3,a5
1c00812c:	8e8d                	sub	a3,a3,a1
1c00812e:	0266f633          	remu	a2,a3,t1
1c008132:	0266d6b3          	divu	a3,a3,t1
1c008136:	df062833          	p.insert	a6,a2,15,16
1c00813a:	02d88733          	mul	a4,a7,a3
1c00813e:	8636                	mv	a2,a3
1c008140:	00e87b63          	bgeu	a6,a4,1c008156 <__udivdi3+0xba>
1c008144:	983e                	add	a6,a6,a5
1c008146:	fff68613          	addi	a2,a3,-1 # ffff <pos_soc_event_callback+0xfbbb>
1c00814a:	00f86663          	bltu	a6,a5,1c008156 <__udivdi3+0xba>
1c00814e:	00e87463          	bgeu	a6,a4,1c008156 <__udivdi3+0xba>
1c008152:	ffe68613          	addi	a2,a3,-2
1c008156:	0542                	slli	a0,a0,0x10
1c008158:	8d51                	or	a0,a0,a2
1c00815a:	4581                	li	a1,0
1c00815c:	a07d                	j	1c00820a <__udivdi3+0x16e>
1c00815e:	01000337          	lui	t1,0x1000
1c008162:	46c1                	li	a3,16
1c008164:	f66661e3          	bltu	a2,t1,1c0080c6 <__udivdi3+0x2a>
1c008168:	46e1                	li	a3,24
1c00816a:	bfb1                	j	1c0080c6 <__udivdi3+0x2a>
1c00816c:	e601                	bnez	a2,1c008174 <__udivdi3+0xd8>
1c00816e:	4685                	li	a3,1
1c008170:	02c6d7b3          	divu	a5,a3,a2
1c008174:	66c1                	lui	a3,0x10
1c008176:	08d7fb63          	bgeu	a5,a3,1c00820c <__udivdi3+0x170>
1c00817a:	0ff00693          	li	a3,255
1c00817e:	00f6f363          	bgeu	a3,a5,1c008184 <__udivdi3+0xe8>
1c008182:	4321                	li	t1,8
1c008184:	0067d6b3          	srl	a3,a5,t1
1c008188:	9536                	add	a0,a0,a3
1c00818a:	00054683          	lbu	a3,0(a0)
1c00818e:	02000593          	li	a1,32
1c008192:	969a                	add	a3,a3,t1
1c008194:	40d58633          	sub	a2,a1,a3
1c008198:	08d59163          	bne	a1,a3,1c00821a <__udivdi3+0x17e>
1c00819c:	8f1d                	sub	a4,a4,a5
1c00819e:	4585                	li	a1,1
1c0081a0:	0107d313          	srli	t1,a5,0x10
1c0081a4:	01079893          	slli	a7,a5,0x10
1c0081a8:	0108d893          	srli	a7,a7,0x10
1c0081ac:	01085693          	srli	a3,a6,0x10
1c0081b0:	02677633          	remu	a2,a4,t1
1c0081b4:	02675733          	divu	a4,a4,t1
1c0081b8:	0642                	slli	a2,a2,0x10
1c0081ba:	8ed1                	or	a3,a3,a2
1c0081bc:	02e88e33          	mul	t3,a7,a4
1c0081c0:	853a                	mv	a0,a4
1c0081c2:	01c6fc63          	bgeu	a3,t3,1c0081da <__udivdi3+0x13e>
1c0081c6:	96be                	add	a3,a3,a5
1c0081c8:	fff70513          	addi	a0,a4,-1
1c0081cc:	00f6e763          	bltu	a3,a5,1c0081da <__udivdi3+0x13e>
1c0081d0:	01c6f563          	bgeu	a3,t3,1c0081da <__udivdi3+0x13e>
1c0081d4:	ffe70513          	addi	a0,a4,-2
1c0081d8:	96be                	add	a3,a3,a5
1c0081da:	41c686b3          	sub	a3,a3,t3
1c0081de:	0266f633          	remu	a2,a3,t1
1c0081e2:	0266d6b3          	divu	a3,a3,t1
1c0081e6:	df062833          	p.insert	a6,a2,15,16
1c0081ea:	02d88733          	mul	a4,a7,a3
1c0081ee:	8636                	mv	a2,a3
1c0081f0:	00e87b63          	bgeu	a6,a4,1c008206 <__udivdi3+0x16a>
1c0081f4:	983e                	add	a6,a6,a5
1c0081f6:	fff68613          	addi	a2,a3,-1 # ffff <pos_soc_event_callback+0xfbbb>
1c0081fa:	00f86663          	bltu	a6,a5,1c008206 <__udivdi3+0x16a>
1c0081fe:	00e87463          	bgeu	a6,a4,1c008206 <__udivdi3+0x16a>
1c008202:	ffe68613          	addi	a2,a3,-2
1c008206:	0542                	slli	a0,a0,0x10
1c008208:	8d51                	or	a0,a0,a2
1c00820a:	8082                	ret
1c00820c:	010006b7          	lui	a3,0x1000
1c008210:	4341                	li	t1,16
1c008212:	f6d7e9e3          	bltu	a5,a3,1c008184 <__udivdi3+0xe8>
1c008216:	4361                	li	t1,24
1c008218:	b7b5                	j	1c008184 <__udivdi3+0xe8>
1c00821a:	00c797b3          	sll	a5,a5,a2
1c00821e:	00d75333          	srl	t1,a4,a3
1c008222:	0107de13          	srli	t3,a5,0x10
1c008226:	00c715b3          	sll	a1,a4,a2
1c00822a:	00c89833          	sll	a6,a7,a2
1c00822e:	00d8d733          	srl	a4,a7,a3
1c008232:	03c358b3          	divu	a7,t1,t3
1c008236:	01079513          	slli	a0,a5,0x10
1c00823a:	8141                	srli	a0,a0,0x10
1c00823c:	8f4d                	or	a4,a4,a1
1c00823e:	01075693          	srli	a3,a4,0x10
1c008242:	03c37633          	remu	a2,t1,t3
1c008246:	85c6                	mv	a1,a7
1c008248:	03150333          	mul	t1,a0,a7
1c00824c:	0642                	slli	a2,a2,0x10
1c00824e:	8ed1                	or	a3,a3,a2
1c008250:	0066fc63          	bgeu	a3,t1,1c008268 <__udivdi3+0x1cc>
1c008254:	96be                	add	a3,a3,a5
1c008256:	fff88593          	addi	a1,a7,-1
1c00825a:	00f6e763          	bltu	a3,a5,1c008268 <__udivdi3+0x1cc>
1c00825e:	0066f563          	bgeu	a3,t1,1c008268 <__udivdi3+0x1cc>
1c008262:	ffe88593          	addi	a1,a7,-2
1c008266:	96be                	add	a3,a3,a5
1c008268:	406686b3          	sub	a3,a3,t1
1c00826c:	03c6f8b3          	remu	a7,a3,t3
1c008270:	03c6d6b3          	divu	a3,a3,t3
1c008274:	df08a733          	p.insert	a4,a7,15,16
1c008278:	02d50633          	mul	a2,a0,a3
1c00827c:	8536                	mv	a0,a3
1c00827e:	00c77c63          	bgeu	a4,a2,1c008296 <__udivdi3+0x1fa>
1c008282:	973e                	add	a4,a4,a5
1c008284:	fff68513          	addi	a0,a3,-1 # ffffff <__l1_heap_size+0xfc001f>
1c008288:	00f76763          	bltu	a4,a5,1c008296 <__udivdi3+0x1fa>
1c00828c:	00c77563          	bgeu	a4,a2,1c008296 <__udivdi3+0x1fa>
1c008290:	ffe68513          	addi	a0,a3,-2
1c008294:	973e                	add	a4,a4,a5
1c008296:	05c2                	slli	a1,a1,0x10
1c008298:	8f11                	sub	a4,a4,a2
1c00829a:	8dc9                	or	a1,a1,a0
1c00829c:	b711                	j	1c0081a0 <__udivdi3+0x104>
1c00829e:	12d5ec63          	bltu	a1,a3,1c0083d6 <__udivdi3+0x33a>
1c0082a2:	67c1                	lui	a5,0x10
1c0082a4:	02f6fd63          	bgeu	a3,a5,1c0082de <__udivdi3+0x242>
1c0082a8:	0ff00793          	li	a5,255
1c0082ac:	00d7b5b3          	sltu	a1,a5,a3
1c0082b0:	058e                	slli	a1,a1,0x3
1c0082b2:	1c0017b7          	lui	a5,0x1c001
1c0082b6:	00b6d533          	srl	a0,a3,a1
1c0082ba:	86878793          	addi	a5,a5,-1944 # 1c000868 <__clz_tab>
1c0082be:	97aa                	add	a5,a5,a0
1c0082c0:	0007c783          	lbu	a5,0(a5)
1c0082c4:	02000513          	li	a0,32
1c0082c8:	97ae                	add	a5,a5,a1
1c0082ca:	40f505b3          	sub	a1,a0,a5
1c0082ce:	00f51f63          	bne	a0,a5,1c0082ec <__udivdi3+0x250>
1c0082d2:	4505                	li	a0,1
1c0082d4:	f2e6ebe3          	bltu	a3,a4,1c00820a <__udivdi3+0x16e>
1c0082d8:	05163533          	p.sletu	a0,a2,a7
1c0082dc:	b73d                	j	1c00820a <__udivdi3+0x16e>
1c0082de:	010007b7          	lui	a5,0x1000
1c0082e2:	45c1                	li	a1,16
1c0082e4:	fcf6e7e3          	bltu	a3,a5,1c0082b2 <__udivdi3+0x216>
1c0082e8:	45e1                	li	a1,24
1c0082ea:	b7e1                	j	1c0082b2 <__udivdi3+0x216>
1c0082ec:	00f65833          	srl	a6,a2,a5
1c0082f0:	00b696b3          	sll	a3,a3,a1
1c0082f4:	00d86833          	or	a6,a6,a3
1c0082f8:	00f75e33          	srl	t3,a4,a5
1c0082fc:	00b61eb3          	sll	t4,a2,a1
1c008300:	01085613          	srli	a2,a6,0x10
1c008304:	02ce76b3          	remu	a3,t3,a2
1c008308:	01081313          	slli	t1,a6,0x10
1c00830c:	01035313          	srli	t1,t1,0x10
1c008310:	00f8d7b3          	srl	a5,a7,a5
1c008314:	00b71733          	sll	a4,a4,a1
1c008318:	8f5d                	or	a4,a4,a5
1c00831a:	01075793          	srli	a5,a4,0x10
1c00831e:	02ce5e33          	divu	t3,t3,a2
1c008322:	06c2                	slli	a3,a3,0x10
1c008324:	8fd5                	or	a5,a5,a3
1c008326:	03c30f33          	mul	t5,t1,t3
1c00832a:	8572                	mv	a0,t3
1c00832c:	01e7fc63          	bgeu	a5,t5,1c008344 <__udivdi3+0x2a8>
1c008330:	97c2                	add	a5,a5,a6
1c008332:	fffe0513          	addi	a0,t3,-1
1c008336:	0107e763          	bltu	a5,a6,1c008344 <__udivdi3+0x2a8>
1c00833a:	01e7f563          	bgeu	a5,t5,1c008344 <__udivdi3+0x2a8>
1c00833e:	ffee0513          	addi	a0,t3,-2
1c008342:	97c2                	add	a5,a5,a6
1c008344:	41e787b3          	sub	a5,a5,t5
1c008348:	02c7fe33          	remu	t3,a5,a2
1c00834c:	02c7d7b3          	divu	a5,a5,a2
1c008350:	df0e2733          	p.insert	a4,t3,15,16
1c008354:	02f306b3          	mul	a3,t1,a5
1c008358:	863e                	mv	a2,a5
1c00835a:	00d77c63          	bgeu	a4,a3,1c008372 <__udivdi3+0x2d6>
1c00835e:	9742                	add	a4,a4,a6
1c008360:	fff78613          	addi	a2,a5,-1 # ffffff <__l1_heap_size+0xfc001f>
1c008364:	01076763          	bltu	a4,a6,1c008372 <__udivdi3+0x2d6>
1c008368:	00d77563          	bgeu	a4,a3,1c008372 <__udivdi3+0x2d6>
1c00836c:	ffe78613          	addi	a2,a5,-2
1c008370:	9742                	add	a4,a4,a6
1c008372:	0542                	slli	a0,a0,0x10
1c008374:	6e41                	lui	t3,0x10
1c008376:	8d51                	or	a0,a0,a2
1c008378:	8f15                	sub	a4,a4,a3
1c00837a:	fffe0693          	addi	a3,t3,-1 # ffff <pos_soc_event_callback+0xfbbb>
1c00837e:	00d577b3          	and	a5,a0,a3
1c008382:	01055613          	srli	a2,a0,0x10
1c008386:	00def6b3          	and	a3,t4,a3
1c00838a:	02d78333          	mul	t1,a5,a3
1c00838e:	010ed813          	srli	a6,t4,0x10
1c008392:	02d606b3          	mul	a3,a2,a3
1c008396:	8eb6                	mv	t4,a3
1c008398:	43078eb3          	p.mac	t4,a5,a6
1c00839c:	01035793          	srli	a5,t1,0x10
1c0083a0:	97f6                	add	a5,a5,t4
1c0083a2:	03060633          	mul	a2,a2,a6
1c0083a6:	00d7f363          	bgeu	a5,a3,1c0083ac <__udivdi3+0x310>
1c0083aa:	9672                	add	a2,a2,t3
1c0083ac:	0107d693          	srli	a3,a5,0x10
1c0083b0:	9636                	add	a2,a2,a3
1c0083b2:	02c76063          	bltu	a4,a2,1c0083d2 <__udivdi3+0x336>
1c0083b6:	dac712e3          	bne	a4,a2,1c00815a <__udivdi3+0xbe>
1c0083ba:	6741                	lui	a4,0x10
1c0083bc:	177d                	addi	a4,a4,-1
1c0083be:	8ff9                	and	a5,a5,a4
1c0083c0:	07c2                	slli	a5,a5,0x10
1c0083c2:	00e37333          	and	t1,t1,a4
1c0083c6:	00b898b3          	sll	a7,a7,a1
1c0083ca:	979a                	add	a5,a5,t1
1c0083cc:	4581                	li	a1,0
1c0083ce:	e2f8fee3          	bgeu	a7,a5,1c00820a <__udivdi3+0x16e>
1c0083d2:	157d                	addi	a0,a0,-1
1c0083d4:	b359                	j	1c00815a <__udivdi3+0xbe>
1c0083d6:	4581                	li	a1,0
1c0083d8:	4501                	li	a0,0
1c0083da:	bd05                	j	1c00820a <__udivdi3+0x16e>

1c0083dc <__umoddi3>:
1c0083dc:	88b2                	mv	a7,a2
1c0083de:	8736                	mv	a4,a3
1c0083e0:	87aa                	mv	a5,a0
1c0083e2:	882e                	mv	a6,a1
1c0083e4:	1a069c63          	bnez	a3,1c00859c <__umoddi3+0x1c0>
1c0083e8:	1c0016b7          	lui	a3,0x1c001
1c0083ec:	86868693          	addi	a3,a3,-1944 # 1c000868 <__clz_tab>
1c0083f0:	0ac5f563          	bgeu	a1,a2,1c00849a <__umoddi3+0xbe>
1c0083f4:	6341                	lui	t1,0x10
1c0083f6:	08667b63          	bgeu	a2,t1,1c00848c <__umoddi3+0xb0>
1c0083fa:	0ff00313          	li	t1,255
1c0083fe:	00c37363          	bgeu	t1,a2,1c008404 <__umoddi3+0x28>
1c008402:	4721                	li	a4,8
1c008404:	00e65333          	srl	t1,a2,a4
1c008408:	969a                	add	a3,a3,t1
1c00840a:	0006c683          	lbu	a3,0(a3)
1c00840e:	96ba                	add	a3,a3,a4
1c008410:	02000713          	li	a4,32
1c008414:	40d70333          	sub	t1,a4,a3
1c008418:	00d70c63          	beq	a4,a3,1c008430 <__umoddi3+0x54>
1c00841c:	006595b3          	sll	a1,a1,t1
1c008420:	00d55733          	srl	a4,a0,a3
1c008424:	006618b3          	sll	a7,a2,t1
1c008428:	00b76833          	or	a6,a4,a1
1c00842c:	006517b3          	sll	a5,a0,t1
1c008430:	0108d613          	srli	a2,a7,0x10
1c008434:	02c87733          	remu	a4,a6,a2
1c008438:	01089513          	slli	a0,a7,0x10
1c00843c:	8141                	srli	a0,a0,0x10
1c00843e:	0107d693          	srli	a3,a5,0x10
1c008442:	02c85833          	divu	a6,a6,a2
1c008446:	0742                	slli	a4,a4,0x10
1c008448:	8ed9                	or	a3,a3,a4
1c00844a:	03050833          	mul	a6,a0,a6
1c00844e:	0106f863          	bgeu	a3,a6,1c00845e <__umoddi3+0x82>
1c008452:	96c6                	add	a3,a3,a7
1c008454:	0116e563          	bltu	a3,a7,1c00845e <__umoddi3+0x82>
1c008458:	0106f363          	bgeu	a3,a6,1c00845e <__umoddi3+0x82>
1c00845c:	96c6                	add	a3,a3,a7
1c00845e:	410686b3          	sub	a3,a3,a6
1c008462:	02c6f733          	remu	a4,a3,a2
1c008466:	02c6d6b3          	divu	a3,a3,a2
1c00846a:	df0727b3          	p.insert	a5,a4,15,16
1c00846e:	02d506b3          	mul	a3,a0,a3
1c008472:	00d7f863          	bgeu	a5,a3,1c008482 <__umoddi3+0xa6>
1c008476:	97c6                	add	a5,a5,a7
1c008478:	0117e563          	bltu	a5,a7,1c008482 <__umoddi3+0xa6>
1c00847c:	00d7f363          	bgeu	a5,a3,1c008482 <__umoddi3+0xa6>
1c008480:	97c6                	add	a5,a5,a7
1c008482:	8f95                	sub	a5,a5,a3
1c008484:	0067d533          	srl	a0,a5,t1
1c008488:	4581                	li	a1,0
1c00848a:	8082                	ret
1c00848c:	01000337          	lui	t1,0x1000
1c008490:	4741                	li	a4,16
1c008492:	f66669e3          	bltu	a2,t1,1c008404 <__umoddi3+0x28>
1c008496:	4761                	li	a4,24
1c008498:	b7b5                	j	1c008404 <__umoddi3+0x28>
1c00849a:	e601                	bnez	a2,1c0084a2 <__umoddi3+0xc6>
1c00849c:	4605                	li	a2,1
1c00849e:	031658b3          	divu	a7,a2,a7
1c0084a2:	6641                	lui	a2,0x10
1c0084a4:	08c8f063          	bgeu	a7,a2,1c008524 <__umoddi3+0x148>
1c0084a8:	0ff00613          	li	a2,255
1c0084ac:	01167363          	bgeu	a2,a7,1c0084b2 <__umoddi3+0xd6>
1c0084b0:	4721                	li	a4,8
1c0084b2:	00e8d633          	srl	a2,a7,a4
1c0084b6:	96b2                	add	a3,a3,a2
1c0084b8:	0006c683          	lbu	a3,0(a3)
1c0084bc:	96ba                	add	a3,a3,a4
1c0084be:	02000713          	li	a4,32
1c0084c2:	40d70333          	sub	t1,a4,a3
1c0084c6:	06d71663          	bne	a4,a3,1c008532 <__umoddi3+0x156>
1c0084ca:	411585b3          	sub	a1,a1,a7
1c0084ce:	0108d713          	srli	a4,a7,0x10
1c0084d2:	01089513          	slli	a0,a7,0x10
1c0084d6:	8141                	srli	a0,a0,0x10
1c0084d8:	0107d613          	srli	a2,a5,0x10
1c0084dc:	02e5f6b3          	remu	a3,a1,a4
1c0084e0:	02e5d5b3          	divu	a1,a1,a4
1c0084e4:	06c2                	slli	a3,a3,0x10
1c0084e6:	8ed1                	or	a3,a3,a2
1c0084e8:	02b505b3          	mul	a1,a0,a1
1c0084ec:	00b6f863          	bgeu	a3,a1,1c0084fc <__umoddi3+0x120>
1c0084f0:	96c6                	add	a3,a3,a7
1c0084f2:	0116e563          	bltu	a3,a7,1c0084fc <__umoddi3+0x120>
1c0084f6:	00b6f363          	bgeu	a3,a1,1c0084fc <__umoddi3+0x120>
1c0084fa:	96c6                	add	a3,a3,a7
1c0084fc:	40b685b3          	sub	a1,a3,a1
1c008500:	02e5f6b3          	remu	a3,a1,a4
1c008504:	02e5d5b3          	divu	a1,a1,a4
1c008508:	df06a7b3          	p.insert	a5,a3,15,16
1c00850c:	02b505b3          	mul	a1,a0,a1
1c008510:	00b7f863          	bgeu	a5,a1,1c008520 <__umoddi3+0x144>
1c008514:	97c6                	add	a5,a5,a7
1c008516:	0117e563          	bltu	a5,a7,1c008520 <__umoddi3+0x144>
1c00851a:	00b7f363          	bgeu	a5,a1,1c008520 <__umoddi3+0x144>
1c00851e:	97c6                	add	a5,a5,a7
1c008520:	8f8d                	sub	a5,a5,a1
1c008522:	b78d                	j	1c008484 <__umoddi3+0xa8>
1c008524:	01000637          	lui	a2,0x1000
1c008528:	4741                	li	a4,16
1c00852a:	f8c8e4e3          	bltu	a7,a2,1c0084b2 <__umoddi3+0xd6>
1c00852e:	4761                	li	a4,24
1c008530:	b749                	j	1c0084b2 <__umoddi3+0xd6>
1c008532:	006898b3          	sll	a7,a7,t1
1c008536:	00d5d733          	srl	a4,a1,a3
1c00853a:	0108d813          	srli	a6,a7,0x10
1c00853e:	03077633          	remu	a2,a4,a6
1c008542:	00d556b3          	srl	a3,a0,a3
1c008546:	006517b3          	sll	a5,a0,t1
1c00854a:	01089513          	slli	a0,a7,0x10
1c00854e:	8141                	srli	a0,a0,0x10
1c008550:	006595b3          	sll	a1,a1,t1
1c008554:	8dd5                	or	a1,a1,a3
1c008556:	0105d693          	srli	a3,a1,0x10
1c00855a:	03075733          	divu	a4,a4,a6
1c00855e:	0642                	slli	a2,a2,0x10
1c008560:	8ed1                	or	a3,a3,a2
1c008562:	02e50733          	mul	a4,a0,a4
1c008566:	00e6f863          	bgeu	a3,a4,1c008576 <__umoddi3+0x19a>
1c00856a:	96c6                	add	a3,a3,a7
1c00856c:	0116e563          	bltu	a3,a7,1c008576 <__umoddi3+0x19a>
1c008570:	00e6f363          	bgeu	a3,a4,1c008576 <__umoddi3+0x19a>
1c008574:	96c6                	add	a3,a3,a7
1c008576:	8e99                	sub	a3,a3,a4
1c008578:	0306f733          	remu	a4,a3,a6
1c00857c:	0306d6b3          	divu	a3,a3,a6
1c008580:	df0725b3          	p.insert	a1,a4,15,16
1c008584:	02d506b3          	mul	a3,a0,a3
1c008588:	00d5f863          	bgeu	a1,a3,1c008598 <__umoddi3+0x1bc>
1c00858c:	95c6                	add	a1,a1,a7
1c00858e:	0115e563          	bltu	a1,a7,1c008598 <__umoddi3+0x1bc>
1c008592:	00d5f363          	bgeu	a1,a3,1c008598 <__umoddi3+0x1bc>
1c008596:	95c6                	add	a1,a1,a7
1c008598:	8d95                	sub	a1,a1,a3
1c00859a:	bf15                	j	1c0084ce <__umoddi3+0xf2>
1c00859c:	eed5e7e3          	bltu	a1,a3,1c00848a <__umoddi3+0xae>
1c0085a0:	6741                	lui	a4,0x10
1c0085a2:	04e6f563          	bgeu	a3,a4,1c0085ec <__umoddi3+0x210>
1c0085a6:	0ff00e13          	li	t3,255
1c0085aa:	00de3733          	sltu	a4,t3,a3
1c0085ae:	070e                	slli	a4,a4,0x3
1c0085b0:	1c0018b7          	lui	a7,0x1c001
1c0085b4:	00e6d333          	srl	t1,a3,a4
1c0085b8:	86888893          	addi	a7,a7,-1944 # 1c000868 <__clz_tab>
1c0085bc:	989a                	add	a7,a7,t1
1c0085be:	0008ce03          	lbu	t3,0(a7)
1c0085c2:	9e3a                	add	t3,t3,a4
1c0085c4:	02000713          	li	a4,32
1c0085c8:	41c70333          	sub	t1,a4,t3
1c0085cc:	03c71763          	bne	a4,t3,1c0085fa <__umoddi3+0x21e>
1c0085d0:	00b6e463          	bltu	a3,a1,1c0085d8 <__umoddi3+0x1fc>
1c0085d4:	00c56963          	bltu	a0,a2,1c0085e6 <__umoddi3+0x20a>
1c0085d8:	40c507b3          	sub	a5,a0,a2
1c0085dc:	8d95                	sub	a1,a1,a3
1c0085de:	00f53533          	sltu	a0,a0,a5
1c0085e2:	40a58833          	sub	a6,a1,a0
1c0085e6:	853e                	mv	a0,a5
1c0085e8:	85c2                	mv	a1,a6
1c0085ea:	b545                	j	1c00848a <__umoddi3+0xae>
1c0085ec:	010008b7          	lui	a7,0x1000
1c0085f0:	4741                	li	a4,16
1c0085f2:	fb16efe3          	bltu	a3,a7,1c0085b0 <__umoddi3+0x1d4>
1c0085f6:	4761                	li	a4,24
1c0085f8:	bf65                	j	1c0085b0 <__umoddi3+0x1d4>
1c0085fa:	01c65733          	srl	a4,a2,t3
1c0085fe:	006696b3          	sll	a3,a3,t1
1c008602:	00d76eb3          	or	t4,a4,a3
1c008606:	01c5d7b3          	srl	a5,a1,t3
1c00860a:	01c558b3          	srl	a7,a0,t3
1c00860e:	010ed713          	srli	a4,t4,0x10
1c008612:	006595b3          	sll	a1,a1,t1
1c008616:	00b8e5b3          	or	a1,a7,a1
1c00861a:	02e7f8b3          	remu	a7,a5,a4
1c00861e:	010e9813          	slli	a6,t4,0x10
1c008622:	01085813          	srli	a6,a6,0x10
1c008626:	0105d693          	srli	a3,a1,0x10
1c00862a:	00661633          	sll	a2,a2,t1
1c00862e:	00651533          	sll	a0,a0,t1
1c008632:	02e7d7b3          	divu	a5,a5,a4
1c008636:	08c2                	slli	a7,a7,0x10
1c008638:	00d8e6b3          	or	a3,a7,a3
1c00863c:	02f80f33          	mul	t5,a6,a5
1c008640:	88be                	mv	a7,a5
1c008642:	01e6fc63          	bgeu	a3,t5,1c00865a <__umoddi3+0x27e>
1c008646:	96f6                	add	a3,a3,t4
1c008648:	fff78893          	addi	a7,a5,-1
1c00864c:	01d6e763          	bltu	a3,t4,1c00865a <__umoddi3+0x27e>
1c008650:	01e6f563          	bgeu	a3,t5,1c00865a <__umoddi3+0x27e>
1c008654:	ffe78893          	addi	a7,a5,-2
1c008658:	96f6                	add	a3,a3,t4
1c00865a:	41e686b3          	sub	a3,a3,t5
1c00865e:	02e6f7b3          	remu	a5,a3,a4
1c008662:	02e6d6b3          	divu	a3,a3,a4
1c008666:	df07a5b3          	p.insert	a1,a5,15,16
1c00866a:	02d80833          	mul	a6,a6,a3
1c00866e:	87b6                	mv	a5,a3
1c008670:	0105fc63          	bgeu	a1,a6,1c008688 <__umoddi3+0x2ac>
1c008674:	95f6                	add	a1,a1,t4
1c008676:	fff68793          	addi	a5,a3,-1
1c00867a:	01d5e763          	bltu	a1,t4,1c008688 <__umoddi3+0x2ac>
1c00867e:	0105f563          	bgeu	a1,a6,1c008688 <__umoddi3+0x2ac>
1c008682:	ffe68793          	addi	a5,a3,-2
1c008686:	95f6                	add	a1,a1,t4
1c008688:	410585b3          	sub	a1,a1,a6
1c00868c:	08c2                	slli	a7,a7,0x10
1c00868e:	6841                	lui	a6,0x10
1c008690:	00f8e8b3          	or	a7,a7,a5
1c008694:	fff80793          	addi	a5,a6,-1 # ffff <pos_soc_event_callback+0xfbbb>
1c008698:	00f8ff33          	and	t5,a7,a5
1c00869c:	0108d893          	srli	a7,a7,0x10
1c0086a0:	8ff1                	and	a5,a5,a2
1c0086a2:	02ff0733          	mul	a4,t5,a5
1c0086a6:	01065693          	srli	a3,a2,0x10
1c0086aa:	02f887b3          	mul	a5,a7,a5
1c0086ae:	8fbe                	mv	t6,a5
1c0086b0:	42df0fb3          	p.mac	t6,t5,a3
1c0086b4:	02d888b3          	mul	a7,a7,a3
1c0086b8:	01075693          	srli	a3,a4,0x10
1c0086bc:	96fe                	add	a3,a3,t6
1c0086be:	00f6f363          	bgeu	a3,a5,1c0086c4 <__umoddi3+0x2e8>
1c0086c2:	98c2                	add	a7,a7,a6
1c0086c4:	67c1                	lui	a5,0x10
1c0086c6:	17fd                	addi	a5,a5,-1
1c0086c8:	0106d813          	srli	a6,a3,0x10
1c0086cc:	8efd                	and	a3,a3,a5
1c0086ce:	06c2                	slli	a3,a3,0x10
1c0086d0:	8f7d                	and	a4,a4,a5
1c0086d2:	98c2                	add	a7,a7,a6
1c0086d4:	9736                	add	a4,a4,a3
1c0086d6:	0115e663          	bltu	a1,a7,1c0086e2 <__umoddi3+0x306>
1c0086da:	01159c63          	bne	a1,a7,1c0086f2 <__umoddi3+0x316>
1c0086de:	00e57a63          	bgeu	a0,a4,1c0086f2 <__umoddi3+0x316>
1c0086e2:	40c70633          	sub	a2,a4,a2
1c0086e6:	00c73733          	sltu	a4,a4,a2
1c0086ea:	9776                	add	a4,a4,t4
1c0086ec:	40e888b3          	sub	a7,a7,a4
1c0086f0:	8732                	mv	a4,a2
1c0086f2:	40e50733          	sub	a4,a0,a4
1c0086f6:	00e53533          	sltu	a0,a0,a4
1c0086fa:	411585b3          	sub	a1,a1,a7
1c0086fe:	8d89                	sub	a1,a1,a0
1c008700:	01c597b3          	sll	a5,a1,t3
1c008704:	00675733          	srl	a4,a4,t1
1c008708:	00e7e533          	or	a0,a5,a4
1c00870c:	0065d5b3          	srl	a1,a1,t1
1c008710:	bbad                	j	1c00848a <__umoddi3+0xae>

1c008712 <maxpool_benchmark_simd_3x3>:
1c008712:	1c0108b7          	lui	a7,0x1c010
1c008716:	1141                	addi	sp,sp,-16
1c008718:	14888893          	addi	a7,a7,328 # 1c010148 <min_results_L2>
1c00871c:	1c010eb7          	lui	t4,0x1c010
1c008720:	6e41                	lui	t3,0x10
1c008722:	c622                	sw	s0,12(sp)
1c008724:	06488313          	addi	t1,a7,100
1c008728:	4801                	li	a6,0
1c00872a:	4581                	li	a1,0
1c00872c:	210e8e93          	addi	t4,t4,528 # 1c010210 <Image_L2>
1c008730:	1e7d                	addi	t3,t3,-1
1c008732:	4f19                	li	t5,6
1c008734:	03100513          	li	a0,49
1c008738:	00381793          	slli	a5,a6,0x3
1c00873c:	97c2                	add	a5,a5,a6
1c00873e:	97ae                	add	a5,a5,a1
1c008740:	0786                	slli	a5,a5,0x1
1c008742:	97f6                	add	a5,a5,t4
1c008744:	0027d403          	lhu	s0,2(a5) # 10002 <pos_soc_event_callback+0xfbbe>
1c008748:	0007d703          	lhu	a4,0(a5)
1c00874c:	0107d383          	lhu	t2,16(a5)
1c008750:	0207d683          	lhu	a3,32(a5)
1c008754:	d0e40757          	pv.pack	a4,s0,a4
1c008758:	0047d283          	lhu	t0,4(a5)
1c00875c:	00e7d603          	lhu	a2,14(a5)
1c008760:	01e7df83          	lhu	t6,30(a5)
1c008764:	00c7d403          	lhu	s0,12(a5)
1c008768:	d0c38657          	pv.pack	a2,t2,a2
1c00876c:	d1f68fd7          	pv.pack	t6,a3,t6
1c008770:	d05402d7          	pv.pack	t0,s0,t0
1c008774:	39f603d7          	pv.maxu.h	t2,a2,t6
1c008778:	385706d7          	pv.maxu.h	a3,a4,t0
1c00877c:	387686d7          	pv.maxu.h	a3,a3,t2
1c008780:	28570757          	pv.minu.h	a4,a4,t0
1c008784:	29f60657          	pv.minu.h	a2,a2,t6
1c008788:	0227df83          	lhu	t6,34(a5)
1c00878c:	28c707d7          	pv.minu.h	a5,a4,a2
1c008790:	01069713          	slli	a4,a3,0x10
1c008794:	8341                	srli	a4,a4,0x10
1c008796:	82c1                	srli	a3,a3,0x10
1c008798:	04d76733          	p.max	a4,a4,a3
1c00879c:	01079693          	slli	a3,a5,0x10
1c0087a0:	82c1                	srli	a3,a3,0x10
1c0087a2:	83c1                	srli	a5,a5,0x10
1c0087a4:	04f6d7b3          	p.minu	a5,a3,a5
1c0087a8:	01c77733          	and	a4,a4,t3
1c0087ac:	05f76733          	p.max	a4,a4,t6
1c0087b0:	04ffdfb3          	p.minu	t6,t6,a5
1c0087b4:	01f8912b          	p.sh	t6,2(a7!)
1c0087b8:	00158793          	addi	a5,a1,1
1c0087bc:	00e3112b          	p.sh	a4,2(t1!) # 1000002 <__l1_heap_size+0xfc0022>
1c0087c0:	00ff5963          	bge	t5,a5,1c0087d2 <maxpool_benchmark_simd_3x3+0xc0>
1c0087c4:	15e9                	addi	a1,a1,-6
1c0087c6:	0805                	addi	a6,a6,1
1c0087c8:	157d                	addi	a0,a0,-1
1c0087ca:	f53d                	bnez	a0,1c008738 <maxpool_benchmark_simd_3x3+0x26>
1c0087cc:	4432                	lw	s0,12(sp)
1c0087ce:	0141                	addi	sp,sp,16
1c0087d0:	8082                	ret
1c0087d2:	85be                	mv	a1,a5
1c0087d4:	157d                	addi	a0,a0,-1
1c0087d6:	f12d                	bnez	a0,1c008738 <maxpool_benchmark_simd_3x3+0x26>
1c0087d8:	bfd5                	j	1c0087cc <maxpool_benchmark_simd_3x3+0xba>

1c0087da <run_benchmark_on_fc>:
1c0087da:	1141                	addi	sp,sp,-16
1c0087dc:	87aa                	mv	a5,a0
1c0087de:	1c001537          	lui	a0,0x1c001
1c0087e2:	c226                	sw	s1,4(sp)
1c0087e4:	96850513          	addi	a0,a0,-1688 # 1c000968 <__clz_tab+0x100>
1c0087e8:	84ae                	mv	s1,a1
1c0087ea:	85be                	mv	a1,a5
1c0087ec:	c606                	sw	ra,12(sp)
1c0087ee:	c422                	sw	s0,8(sp)
1c0087f0:	2e7d                	jal	1c008bae <printf>
1c0087f2:	000207b7          	lui	a5,0x20
1c0087f6:	cc079073          	csrw	0xcc0,a5
1c0087fa:	f1402473          	csrr	s0,mhartid
1c0087fe:	47fd                	li	a5,31
1c008800:	ca541733          	p.extractu	a4,s0,5,5
1c008804:	08f70263          	beq	a4,a5,1c008888 <run_benchmark_on_fc+0xae>
1c008808:	102007b7          	lui	a5,0x10200
1c00880c:	4705                	li	a4,1
1c00880e:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c008812:	02e7a023          	sw	a4,32(a5)
1c008816:	4781                	li	a5,0
1c008818:	79f79073          	csrw	0x79f,a5
1c00881c:	477d                	li	a4,31
1c00881e:	ca5417b3          	p.extractu	a5,s0,5,5
1c008822:	08e78063          	beq	a5,a4,1c0088a2 <run_benchmark_on_fc+0xc8>
1c008826:	102007b7          	lui	a5,0x10200
1c00882a:	4705                	li	a4,1
1c00882c:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c008830:	00e7ac23          	sw	a4,24(a5)
1c008834:	478d                	li	a5,3
1c008836:	cc179073          	csrw	0xcc1,a5
1c00883a:	9482                	jalr	s1
1c00883c:	ca5417b3          	p.extractu	a5,s0,5,5
1c008840:	477d                	li	a4,31
1c008842:	06e78e63          	beq	a5,a4,1c0088be <run_benchmark_on_fc+0xe4>
1c008846:	102007b7          	lui	a5,0x10200
1c00884a:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c00884e:	0007a023          	sw	zero,0(a5)
1c008852:	4781                	li	a5,0
1c008854:	cc179073          	csrw	0xcc1,a5
1c008858:	47fd                	li	a5,31
1c00885a:	ca541433          	p.extractu	s0,s0,5,5
1c00885e:	06f40c63          	beq	s0,a5,1c0088d6 <run_benchmark_on_fc+0xfc>
1c008862:	102005b7          	lui	a1,0x10200
1c008866:	40058593          	addi	a1,a1,1024 # 10200400 <__l1_end+0x2003e0>
1c00886a:	0085a583          	lw	a1,8(a1)
1c00886e:	4422                	lw	s0,8(sp)
1c008870:	40b2                	lw	ra,12(sp)
1c008872:	4492                	lw	s1,4(sp)
1c008874:	00259793          	slli	a5,a1,0x2
1c008878:	95be                	add	a1,a1,a5
1c00887a:	1c001537          	lui	a0,0x1c001
1c00887e:	058a                	slli	a1,a1,0x2
1c008880:	98450513          	addi	a0,a0,-1660 # 1c000984 <__clz_tab+0x11c>
1c008884:	0141                	addi	sp,sp,16
1c008886:	a625                	j	1c008bae <printf>
1c008888:	4785                	li	a5,1
1c00888a:	1a10b737          	lui	a4,0x1a10b
1c00888e:	02f72023          	sw	a5,32(a4) # 1a10b020 <__l1_end+0xa10b000>
1c008892:	4781                	li	a5,0
1c008894:	79f79073          	csrw	0x79f,a5
1c008898:	477d                	li	a4,31
1c00889a:	ca5417b3          	p.extractu	a5,s0,5,5
1c00889e:	f8e794e3          	bne	a5,a4,1c008826 <run_benchmark_on_fc+0x4c>
1c0088a2:	4785                	li	a5,1
1c0088a4:	1a10b737          	lui	a4,0x1a10b
1c0088a8:	00f72c23          	sw	a5,24(a4) # 1a10b018 <__l1_end+0xa10aff8>
1c0088ac:	478d                	li	a5,3
1c0088ae:	cc179073          	csrw	0xcc1,a5
1c0088b2:	9482                	jalr	s1
1c0088b4:	ca5417b3          	p.extractu	a5,s0,5,5
1c0088b8:	477d                	li	a4,31
1c0088ba:	f8e796e3          	bne	a5,a4,1c008846 <run_benchmark_on_fc+0x6c>
1c0088be:	1a10b7b7          	lui	a5,0x1a10b
1c0088c2:	0007a023          	sw	zero,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c0088c6:	4781                	li	a5,0
1c0088c8:	cc179073          	csrw	0xcc1,a5
1c0088cc:	47fd                	li	a5,31
1c0088ce:	ca541433          	p.extractu	s0,s0,5,5
1c0088d2:	f8f418e3          	bne	s0,a5,1c008862 <run_benchmark_on_fc+0x88>
1c0088d6:	1a10b5b7          	lui	a1,0x1a10b
1c0088da:	0085a583          	lw	a1,8(a1) # 1a10b008 <__l1_end+0xa10afe8>
1c0088de:	bf41                	j	1c00886e <run_benchmark_on_fc+0x94>

1c0088e0 <main>:
1c0088e0:	f14027f3          	csrr	a5,mhartid
1c0088e4:	ca5797b3          	p.extractu	a5,a5,5,5
1c0088e8:	e399                	bnez	a5,1c0088ee <main+0xe>
1c0088ea:	4501                	li	a0,0
1c0088ec:	8082                	ret
1c0088ee:	1141                	addi	sp,sp,-16
1c0088f0:	45e5                	li	a1,25
1c0088f2:	02000513          	li	a0,32
1c0088f6:	c606                	sw	ra,12(sp)
1c0088f8:	2089                	jal	1c00893a <padctrl_mode_set>
1c0088fa:	45e9                	li	a1,26
1c0088fc:	02100513          	li	a0,33
1c008900:	282d                	jal	1c00893a <padctrl_mode_set>
1c008902:	1c001537          	lui	a0,0x1c001
1c008906:	9d450513          	addi	a0,a0,-1580 # 1c0009d4 <__clz_tab+0x16c>
1c00890a:	2455                	jal	1c008bae <printf>
1c00890c:	1c0085b7          	lui	a1,0x1c008
1c008910:	1c001537          	lui	a0,0x1c001
1c008914:	9f050513          	addi	a0,a0,-1552 # 1c0009f0 <__clz_tab+0x188>
1c008918:	71258593          	addi	a1,a1,1810 # 1c008712 <maxpool_benchmark_simd_3x3>
1c00891c:	3d7d                	jal	1c0087da <run_benchmark_on_fc>
1c00891e:	40b2                	lw	ra,12(sp)
1c008920:	4501                	li	a0,0
1c008922:	0141                	addi	sp,sp,16
1c008924:	8082                	ret

1c008926 <pos_soc_init>:
1c008926:	02faf737          	lui	a4,0x2faf
1c00892a:	00400793          	li	a5,4
1c00892e:	08070713          	addi	a4,a4,128 # 2faf080 <__l1_heap_size+0x2f6f0a0>
1c008932:	c398                	sw	a4,0(a5)
1c008934:	c798                	sw	a4,8(a5)
1c008936:	c3d8                	sw	a4,4(a5)
1c008938:	8082                	ret

1c00893a <padctrl_mode_set>:
1c00893a:	034287b7          	lui	a5,0x3428
1c00893e:	953e                	add	a0,a0,a5
1c008940:	050e                	slli	a0,a0,0x3
1c008942:	415c                	lw	a5,4(a0)
1c008944:	0511                	addi	a0,a0,4
1c008946:	c805a7b3          	p.insert	a5,a1,4,0
1c00894a:	c11c                	sw	a5,0(a0)
1c00894c:	8082                	ret

1c00894e <pos_libc_putc_uart_req>:
1c00894e:	410c                	lw	a1,0(a0)
1c008950:	1141                	addi	sp,sp,-16
1c008952:	c422                	sw	s0,8(sp)
1c008954:	00759793          	slli	a5,a1,0x7
1c008958:	1c0105b7          	lui	a1,0x1c010
1c00895c:	2b458593          	addi	a1,a1,692 # 1c0102b4 <pos_libc_uart_buffer_cl>
1c008960:	c606                	sw	ra,12(sp)
1c008962:	95be                	add	a1,a1,a5
1c008964:	0005c783          	lbu	a5,0(a1)
1c008968:	842a                	mv	s0,a0
1c00896a:	cf95                	beqz	a5,1c0089a6 <pos_libc_putc_uart_req+0x58>
1c00896c:	00158793          	addi	a5,a1,1
1c008970:	0007c703          	lbu	a4,0(a5) # 3428000 <__l1_heap_size+0x33e8020>
1c008974:	863e                	mv	a2,a5
1c008976:	0785                	addi	a5,a5,1
1c008978:	ff65                	bnez	a4,1c008970 <pos_libc_putc_uart_req+0x22>
1c00897a:	8e0d                	sub	a2,a2,a1
1c00897c:	01000513          	li	a0,16
1c008980:	025020ef          	jal	ra,1c00b1a4 <pi_uart_write>
1c008984:	401c                	lw	a5,0(s0)
1c008986:	4705                	li	a4,1
1c008988:	c058                	sw	a4,4(s0)
1c00898a:	04078793          	addi	a5,a5,64
1c00898e:	00201737          	lui	a4,0x201
1c008992:	40b2                	lw	ra,12(sp)
1c008994:	4422                	lw	s0,8(sp)
1c008996:	07da                	slli	a5,a5,0x16
1c008998:	e0470713          	addi	a4,a4,-508 # 200e04 <__l1_heap_size+0x1c0e24>
1c00899c:	97ba                	add	a5,a5,a4
1c00899e:	0007a023          	sw	zero,0(a5)
1c0089a2:	0141                	addi	sp,sp,16
1c0089a4:	8082                	ret
1c0089a6:	4601                	li	a2,0
1c0089a8:	bfd1                	j	1c00897c <pos_libc_putc_uart_req+0x2e>

1c0089aa <strchr>:
1c0089aa:	00054703          	lbu	a4,0(a0)
1c0089ae:	0ff5f593          	andi	a1,a1,255
1c0089b2:	87aa                	mv	a5,a0
1c0089b4:	00b70863          	beq	a4,a1,1c0089c4 <strchr+0x1a>
1c0089b8:	cb01                	beqz	a4,1c0089c8 <strchr+0x1e>
1c0089ba:	0017c703          	lbu	a4,1(a5)
1c0089be:	0785                	addi	a5,a5,1
1c0089c0:	feb71ce3          	bne	a4,a1,1c0089b8 <strchr+0xe>
1c0089c4:	853e                	mv	a0,a5
1c0089c6:	8082                	ret
1c0089c8:	4501                	li	a0,0
1c0089ca:	dded                	beqz	a1,1c0089c4 <strchr+0x1a>
1c0089cc:	8082                	ret

1c0089ce <pos_libc_fputc_locked>:
1c0089ce:	1c0017b7          	lui	a5,0x1c001
1c0089d2:	2507a783          	lw	a5,592(a5) # 1c001250 <pos_io_uart_enabled>
1c0089d6:	c7a9                	beqz	a5,1c008a20 <pos_libc_fputc_locked+0x52>
1c0089d8:	f1402673          	csrr	a2,mhartid
1c0089dc:	477d                	li	a4,31
1c0089de:	ca5617b3          	p.extractu	a5,a2,5,5
1c0089e2:	08e78063          	beq	a5,a4,1c008a62 <pos_libc_fputc_locked+0x94>
1c0089e6:	00779693          	slli	a3,a5,0x7
1c0089ea:	00279713          	slli	a4,a5,0x2
1c0089ee:	1c0105b7          	lui	a1,0x1c010
1c0089f2:	1c0017b7          	lui	a5,0x1c001
1c0089f6:	2b458593          	addi	a1,a1,692 # 1c0102b4 <pos_libc_uart_buffer_cl>
1c0089fa:	25878793          	addi	a5,a5,600 # 1c001258 <pos_libc_uart_buffer_index_cl>
1c0089fe:	95b6                	add	a1,a1,a3
1c008a00:	97ba                	add	a5,a5,a4
1c008a02:	4398                	lw	a4,0(a5)
1c008a04:	0ff57513          	andi	a0,a0,255
1c008a08:	07f00813          	li	a6,127
1c008a0c:	00170693          	addi	a3,a4,1
1c008a10:	972e                	add	a4,a4,a1
1c008a12:	c394                	sw	a3,0(a5)
1c008a14:	00a70023          	sb	a0,0(a4)
1c008a18:	01068663          	beq	a3,a6,1c008a24 <pos_libc_fputc_locked+0x56>
1c008a1c:	00a52463          	p.beqimm	a0,10,1c008a24 <pos_libc_fputc_locked+0x56>
1c008a20:	4501                	li	a0,0
1c008a22:	8082                	ret
1c008a24:	711d                	addi	sp,sp,-96
1c008a26:	ce86                	sw	ra,92(sp)
1c008a28:	96ae                	add	a3,a3,a1
1c008a2a:	0007a023          	sw	zero,0(a5)
1c008a2e:	00068023          	sb	zero,0(a3)
1c008a32:	ca561633          	p.extractu	a2,a2,5,5
1c008a36:	47fd                	li	a5,31
1c008a38:	02f61e63          	bne	a2,a5,1c008a74 <pos_libc_fputc_locked+0xa6>
1c008a3c:	0005c783          	lbu	a5,0(a1)
1c008a40:	cba5                	beqz	a5,1c008ab0 <pos_libc_fputc_locked+0xe2>
1c008a42:	00158793          	addi	a5,a1,1
1c008a46:	0007c703          	lbu	a4,0(a5)
1c008a4a:	863e                	mv	a2,a5
1c008a4c:	0785                	addi	a5,a5,1
1c008a4e:	ff65                	bnez	a4,1c008a46 <pos_libc_fputc_locked+0x78>
1c008a50:	8e0d                	sub	a2,a2,a1
1c008a52:	01000513          	li	a0,16
1c008a56:	74e020ef          	jal	ra,1c00b1a4 <pi_uart_write>
1c008a5a:	40f6                	lw	ra,92(sp)
1c008a5c:	4501                	li	a0,0
1c008a5e:	6125                	addi	sp,sp,96
1c008a60:	8082                	ret
1c008a62:	1c0017b7          	lui	a5,0x1c001
1c008a66:	1c0105b7          	lui	a1,0x1c010
1c008a6a:	25478793          	addi	a5,a5,596 # 1c001254 <pos_libc_uart_buffer_index>
1c008a6e:	33458593          	addi	a1,a1,820 # 1c010334 <pos_libc_uart_buffer>
1c008a72:	bf41                	j	1c008a02 <pos_libc_fputc_locked+0x34>
1c008a74:	1c0097b7          	lui	a5,0x1c009
1c008a78:	94e78793          	addi	a5,a5,-1714 # 1c00894e <pos_libc_putc_uart_req>
1c008a7c:	ca3e                	sw	a5,20(sp)
1c008a7e:	0808                	addi	a0,sp,16
1c008a80:	003c                	addi	a5,sp,8
1c008a82:	c432                	sw	a2,8(sp)
1c008a84:	cc3e                	sw	a5,24(sp)
1c008a86:	c602                	sw	zero,12(sp)
1c008a88:	055020ef          	jal	ra,1c00b2dc <pos_cluster_push_fc_event>
1c008a8c:	46b2                	lw	a3,12(sp)
1c008a8e:	002047b7          	lui	a5,0x204
1c008a92:	4709                	li	a4,2
1c008a94:	03c78613          	addi	a2,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c008a98:	f2e9                	bnez	a3,1c008a5a <pos_libc_fputc_locked+0x8c>
1c008a9a:	00e7a423          	sw	a4,8(a5)
1c008a9e:	4214                	lw	a3,0(a2)
1c008aa0:	00e7a223          	sw	a4,4(a5)
1c008aa4:	46b2                	lw	a3,12(sp)
1c008aa6:	daf5                	beqz	a3,1c008a9a <pos_libc_fputc_locked+0xcc>
1c008aa8:	40f6                	lw	ra,92(sp)
1c008aaa:	4501                	li	a0,0
1c008aac:	6125                	addi	sp,sp,96
1c008aae:	8082                	ret
1c008ab0:	4601                	li	a2,0
1c008ab2:	b745                	j	1c008a52 <pos_libc_fputc_locked+0x84>

1c008ab4 <pos_libc_prf_locked>:
1c008ab4:	1141                	addi	sp,sp,-16
1c008ab6:	c422                	sw	s0,8(sp)
1c008ab8:	f1402473          	csrr	s0,mhartid
1c008abc:	c606                	sw	ra,12(sp)
1c008abe:	47fd                	li	a5,31
1c008ac0:	ca541733          	p.extractu	a4,s0,5,5
1c008ac4:	02f70963          	beq	a4,a5,1c008af6 <pos_libc_prf_locked+0x42>
1c008ac8:	00400893          	li	a7,4
1c008acc:	c148c8b3          	p.bset	a7,a7,0,20
1c008ad0:	0008a783          	lw	a5,0(a7)
1c008ad4:	03f7b163          	p.bneimm	a5,-1,1c008af6 <pos_libc_prf_locked+0x42>
1c008ad8:	002047b7          	lui	a5,0x204
1c008adc:	4711                	li	a4,4
1c008ade:	03c78313          	addi	t1,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c008ae2:	00e7a423          	sw	a4,8(a5)
1c008ae6:	00032803          	lw	a6,0(t1)
1c008aea:	00e7a223          	sw	a4,4(a5)
1c008aee:	0008a803          	lw	a6,0(a7)
1c008af2:	fff828e3          	p.beqimm	a6,-1,1c008ae2 <pos_libc_prf_locked+0x2e>
1c008af6:	20dd                	jal	1c008bdc <pos_libc_prf>
1c008af8:	ca541433          	p.extractu	s0,s0,5,5
1c008afc:	47fd                	li	a5,31
1c008afe:	00f40863          	beq	s0,a5,1c008b0e <pos_libc_prf_locked+0x5a>
1c008b02:	00002223          	sw	zero,4(zero) # 4 <pos_freq_domains>
1c008b06:	002047b7          	lui	a5,0x204
1c008b0a:	1007a423          	sw	zero,264(a5) # 204108 <__l1_heap_size+0x1c4128>
1c008b0e:	40b2                	lw	ra,12(sp)
1c008b10:	4422                	lw	s0,8(sp)
1c008b12:	0141                	addi	sp,sp,16
1c008b14:	8082                	ret

1c008b16 <exit>:
1c008b16:	1141                	addi	sp,sp,-16
1c008b18:	c422                	sw	s0,8(sp)
1c008b1a:	c606                	sw	ra,12(sp)
1c008b1c:	842a                	mv	s0,a0
1c008b1e:	106020ef          	jal	ra,1c00ac24 <pos_init_stop>
1c008b22:	c1f44433          	p.bset	s0,s0,0,31
1c008b26:	1a1047b7          	lui	a5,0x1a104
1c008b2a:	0a87a023          	sw	s0,160(a5) # 1a1040a0 <__l1_end+0xa104080>
1c008b2e:	1a10a7b7          	lui	a5,0x1a10a
1c008b32:	577d                	li	a4,-1
1c008b34:	80e7a423          	sw	a4,-2040(a5) # 1a109808 <__l1_end+0xa1097e8>
1c008b38:	10500073          	wfi
1c008b3c:	10500073          	wfi
1c008b40:	bfe5                	j	1c008b38 <exit+0x22>

1c008b42 <pos_io_start>:
1c008b42:	1101                	addi	sp,sp,-32
1c008b44:	0048                	addi	a0,sp,4
1c008b46:	ce06                	sw	ra,28(sp)
1c008b48:	cc22                	sw	s0,24(sp)
1c008b4a:	ca26                	sw	s1,20(sp)
1c008b4c:	46a020ef          	jal	ra,1c00afb6 <pi_uart_conf_init>
1c008b50:	67f1                	lui	a5,0x1c
1c008b52:	01000513          	li	a0,16
1c008b56:	20078793          	addi	a5,a5,512 # 1c200 <pos_soc_event_callback+0x1bdbc>
1c008b5a:	004c                	addi	a1,sp,4
1c008b5c:	4485                	li	s1,1
1c008b5e:	00911623          	sh	s1,12(sp)
1c008b62:	c23e                	sw	a5,4(sp)
1c008b64:	102020ef          	jal	ra,1c00ac66 <pi_open_from_conf>
1c008b68:	01000513          	li	a0,16
1c008b6c:	462020ef          	jal	ra,1c00afce <pi_uart_open>
1c008b70:	e911                	bnez	a0,1c008b84 <pos_io_start+0x42>
1c008b72:	1c0017b7          	lui	a5,0x1c001
1c008b76:	2497a823          	sw	s1,592(a5) # 1c001250 <pos_io_uart_enabled>
1c008b7a:	40f2                	lw	ra,28(sp)
1c008b7c:	4462                	lw	s0,24(sp)
1c008b7e:	44d2                	lw	s1,20(sp)
1c008b80:	6105                	addi	sp,sp,32
1c008b82:	8082                	ret
1c008b84:	557d                	li	a0,-1
1c008b86:	bfd5                	j	1c008b7a <pos_io_start+0x38>

1c008b88 <pos_io_stop>:
1c008b88:	1141                	addi	sp,sp,-16
1c008b8a:	01000513          	li	a0,16
1c008b8e:	1c0017b7          	lui	a5,0x1c001
1c008b92:	c606                	sw	ra,12(sp)
1c008b94:	2407a823          	sw	zero,592(a5) # 1c001250 <pos_io_uart_enabled>
1c008b98:	55a020ef          	jal	ra,1c00b0f2 <pi_uart_close>
1c008b9c:	40b2                	lw	ra,12(sp)
1c008b9e:	4501                	li	a0,0
1c008ba0:	0141                	addi	sp,sp,16
1c008ba2:	8082                	ret

1c008ba4 <pos_io_init>:
1c008ba4:	1c0017b7          	lui	a5,0x1c001
1c008ba8:	2407a823          	sw	zero,592(a5) # 1c001250 <pos_io_uart_enabled>
1c008bac:	8082                	ret

1c008bae <printf>:
1c008bae:	7139                	addi	sp,sp,-64
1c008bb0:	02410313          	addi	t1,sp,36
1c008bb4:	d432                	sw	a2,40(sp)
1c008bb6:	862a                	mv	a2,a0
1c008bb8:	1c009537          	lui	a0,0x1c009
1c008bbc:	d22e                	sw	a1,36(sp)
1c008bbe:	d636                	sw	a3,44(sp)
1c008bc0:	4589                	li	a1,2
1c008bc2:	869a                	mv	a3,t1
1c008bc4:	9ce50513          	addi	a0,a0,-1586 # 1c0089ce <pos_libc_fputc_locked>
1c008bc8:	ce06                	sw	ra,28(sp)
1c008bca:	d83a                	sw	a4,48(sp)
1c008bcc:	da3e                	sw	a5,52(sp)
1c008bce:	dc42                	sw	a6,56(sp)
1c008bd0:	de46                	sw	a7,60(sp)
1c008bd2:	c61a                	sw	t1,12(sp)
1c008bd4:	35c5                	jal	1c008ab4 <pos_libc_prf_locked>
1c008bd6:	40f2                	lw	ra,28(sp)
1c008bd8:	6121                	addi	sp,sp,64
1c008bda:	8082                	ret

1c008bdc <pos_libc_prf>:
1c008bdc:	7175                	addi	sp,sp,-144
1c008bde:	dad6                	sw	s5,116(sp)
1c008be0:	c706                	sw	ra,140(sp)
1c008be2:	c522                	sw	s0,136(sp)
1c008be4:	c326                	sw	s1,132(sp)
1c008be6:	c14a                	sw	s2,128(sp)
1c008be8:	dece                	sw	s3,124(sp)
1c008bea:	dcd2                	sw	s4,120(sp)
1c008bec:	d8da                	sw	s6,112(sp)
1c008bee:	d6de                	sw	s7,108(sp)
1c008bf0:	d4e2                	sw	s8,104(sp)
1c008bf2:	d2e6                	sw	s9,100(sp)
1c008bf4:	d0ea                	sw	s10,96(sp)
1c008bf6:	ceee                	sw	s11,92(sp)
1c008bf8:	8ab2                	mv	s5,a2
1c008bfa:	001ac78b          	p.lbu	a5,1(s5!)
1c008bfe:	cc36                	sw	a3,24(sp)
1c008c00:	780786e3          	beqz	a5,1c009b8c <pos_libc_prf+0xfb0>
1c008c04:	7761                	lui	a4,0xffff8
1c008c06:	83074713          	xori	a4,a4,-2000
1c008c0a:	ce3a                	sw	a4,28(sp)
1c008c0c:	1858                	addi	a4,sp,52
1c008c0e:	fff74713          	not	a4,a4
1c008c12:	d23a                	sw	a4,36(sp)
1c008c14:	03610713          	addi	a4,sp,54
1c008c18:	fff74713          	not	a4,a4
1c008c1c:	8a2a                	mv	s4,a0
1c008c1e:	84ae                	mv	s1,a1
1c008c20:	4b01                	li	s6,0
1c008c22:	d03a                	sw	a4,32(sp)
1c008c24:	8d56                	mv	s10,s5
1c008c26:	853e                	mv	a0,a5
1c008c28:	a819                	j	1c008c3e <pos_libc_prf+0x62>
1c008c2a:	85a6                	mv	a1,s1
1c008c2c:	9a02                	jalr	s4
1c008c2e:	19f52763          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008c32:	0b05                	addi	s6,s6,1
1c008c34:	8cea                	mv	s9,s10
1c008c36:	8d66                	mv	s10,s9
1c008c38:	001d450b          	p.lbu	a0,1(s10!)
1c008c3c:	cd29                	beqz	a0,1c008c96 <pos_libc_prf+0xba>
1c008c3e:	02500793          	li	a5,37
1c008c42:	fef514e3          	bne	a0,a5,1c008c2a <pos_libc_prf+0x4e>
1c008c46:	4c01                	li	s8,0
1c008c48:	c802                	sw	zero,16(sp)
1c008c4a:	ca02                	sw	zero,20(sp)
1c008c4c:	c602                	sw	zero,12(sp)
1c008c4e:	4901                	li	s2,0
1c008c50:	02b00413          	li	s0,43
1c008c54:	02d00a93          	li	s5,45
1c008c58:	03000d93          	li	s11,48
1c008c5c:	02000993          	li	s3,32
1c008c60:	000d4b83          	lbu	s7,0(s10)
1c008c64:	1c0017b7          	lui	a5,0x1c001
1c008c68:	9f878513          	addi	a0,a5,-1544 # 1c0009f8 <__clz_tab+0x190>
1c008c6c:	85de                	mv	a1,s7
1c008c6e:	3b35                	jal	1c0089aa <strchr>
1c008c70:	001d0c93          	addi	s9,s10,1
1c008c74:	86de                	mv	a3,s7
1c008c76:	c13d                	beqz	a0,1c008cdc <pos_libc_prf+0x100>
1c008c78:	048b8e63          	beq	s7,s0,1c008cd4 <pos_libc_prf+0xf8>
1c008c7c:	03746d63          	bltu	s0,s7,1c008cb6 <pos_libc_prf+0xda>
1c008c80:	053b8663          	beq	s7,s3,1c008ccc <pos_libc_prf+0xf0>
1c008c84:	02300793          	li	a5,35
1c008c88:	00fb9563          	bne	s7,a5,1c008c92 <pos_libc_prf+0xb6>
1c008c8c:	4905                	li	s2,1
1c008c8e:	8d66                	mv	s10,s9
1c008c90:	bfc1                	j	1c008c60 <pos_libc_prf+0x84>
1c008c92:	fe0b9ee3          	bnez	s7,1c008c8e <pos_libc_prf+0xb2>
1c008c96:	40ba                	lw	ra,140(sp)
1c008c98:	442a                	lw	s0,136(sp)
1c008c9a:	449a                	lw	s1,132(sp)
1c008c9c:	490a                	lw	s2,128(sp)
1c008c9e:	59f6                	lw	s3,124(sp)
1c008ca0:	5a66                	lw	s4,120(sp)
1c008ca2:	5ad6                	lw	s5,116(sp)
1c008ca4:	5bb6                	lw	s7,108(sp)
1c008ca6:	5c26                	lw	s8,104(sp)
1c008ca8:	5c96                	lw	s9,100(sp)
1c008caa:	5d06                	lw	s10,96(sp)
1c008cac:	4df6                	lw	s11,92(sp)
1c008cae:	855a                	mv	a0,s6
1c008cb0:	5b46                	lw	s6,112(sp)
1c008cb2:	6149                	addi	sp,sp,144
1c008cb4:	8082                	ret
1c008cb6:	015b8763          	beq	s7,s5,1c008cc4 <pos_libc_prf+0xe8>
1c008cba:	fdbb9ae3          	bne	s7,s11,1c008c8e <pos_libc_prf+0xb2>
1c008cbe:	4c05                	li	s8,1
1c008cc0:	8d66                	mv	s10,s9
1c008cc2:	bf79                	j	1c008c60 <pos_libc_prf+0x84>
1c008cc4:	4785                	li	a5,1
1c008cc6:	c63e                	sw	a5,12(sp)
1c008cc8:	8d66                	mv	s10,s9
1c008cca:	bf59                	j	1c008c60 <pos_libc_prf+0x84>
1c008ccc:	4785                	li	a5,1
1c008cce:	c83e                	sw	a5,16(sp)
1c008cd0:	8d66                	mv	s10,s9
1c008cd2:	b779                	j	1c008c60 <pos_libc_prf+0x84>
1c008cd4:	4785                	li	a5,1
1c008cd6:	ca3e                	sw	a5,20(sp)
1c008cd8:	8d66                	mv	s10,s9
1c008cda:	b759                	j	1c008c60 <pos_libc_prf+0x84>
1c008cdc:	d64a                	sw	s2,44(sp)
1c008cde:	02a00793          	li	a5,42
1c008ce2:	8966                	mv	s2,s9
1c008ce4:	10fb8263          	beq	s7,a5,1c008de8 <pos_libc_prf+0x20c>
1c008ce8:	fd0b8793          	addi	a5,s7,-48
1c008cec:	4525                	li	a0,9
1c008cee:	4d81                	li	s11,0
1c008cf0:	0cf57963          	bgeu	a0,a5,1c008dc2 <pos_libc_prf+0x1e6>
1c008cf4:	02e00793          	li	a5,46
1c008cf8:	54f68a63          	beq	a3,a5,1c00924c <pos_libc_prf+0x670>
1c008cfc:	89b6                	mv	s3,a3
1c008cfe:	547d                	li	s0,-1
1c008d00:	1c0017b7          	lui	a5,0x1c001
1c008d04:	85b6                	mv	a1,a3
1c008d06:	a0078513          	addi	a0,a5,-1536 # 1c000a00 <__clz_tab+0x198>
1c008d0a:	d436                	sw	a3,40(sp)
1c008d0c:	3979                	jal	1c0089aa <strchr>
1c008d0e:	56a2                	lw	a3,40(sp)
1c008d10:	4601                	li	a2,0
1c008d12:	cd11                	beqz	a0,1c008d2e <pos_libc_prf+0x152>
1c008d14:	87e6                	mv	a5,s9
1c008d16:	06c00713          	li	a4,108
1c008d1a:	0017c68b          	p.lbu	a3,1(a5!)
1c008d1e:	54e98763          	beq	s3,a4,1c00926c <pos_libc_prf+0x690>
1c008d22:	06800713          	li	a4,104
1c008d26:	54e98963          	beq	s3,a4,1c009278 <pos_libc_prf+0x69c>
1c008d2a:	8cbe                	mv	s9,a5
1c008d2c:	864e                	mv	a2,s3
1c008d2e:	06700793          	li	a5,103
1c008d32:	2ed7e963          	bltu	a5,a3,1c009024 <pos_libc_prf+0x448>
1c008d36:	06400793          	li	a5,100
1c008d3a:	3ad7ede3          	bltu	a5,a3,1c0098f4 <pos_libc_prf+0xd18>
1c008d3e:	04700713          	li	a4,71
1c008d42:	0cd76263          	bltu	a4,a3,1c008e06 <pos_libc_prf+0x22a>
1c008d46:	04400793          	li	a5,68
1c008d4a:	3ad7e5e3          	bltu	a5,a3,1c0098f4 <pos_libc_prf+0xd18>
1c008d4e:	d6a1                	beqz	a3,1c008c96 <pos_libc_prf+0xba>
1c008d50:	02500793          	li	a5,37
1c008d54:	46f69d63          	bne	a3,a5,1c0091ce <pos_libc_prf+0x5f2>
1c008d58:	85a6                	mv	a1,s1
1c008d5a:	02500513          	li	a0,37
1c008d5e:	9a02                	jalr	s4
1c008d60:	05f52e63          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008d64:	0b05                	addi	s6,s6,1
1c008d66:	bdc1                	j	1c008c36 <pos_libc_prf+0x5a>
1c008d68:	5722                	lw	a4,40(sp)
1c008d6a:	8bbe                	mv	s7,a5
1c008d6c:	fff70c13          	addi	s8,a4,-1 # ffff7fff <__l2_shared_end+0xe3fe7bf7>
1c008d70:	85a6                	mv	a1,s1
1c008d72:	03000513          	li	a0,48
1c008d76:	9a02                	jalr	s4
1c008d78:	1c7d                	addi	s8,s8,-1
1c008d7a:	05f52163          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008d7e:	fffc39e3          	p.bneimm	s8,-1,1c008d70 <pos_libc_prf+0x194>
1c008d82:	5722                	lw	a4,40(sp)
1c008d84:	40eb8c33          	sub	s8,s7,a4
1c008d88:	640401e3          	beqz	s0,1c009bca <pos_libc_prf+0xfee>
1c008d8c:	000ac703          	lbu	a4,0(s5)
1c008d90:	8dd6                	mv	s11,s5
1c008d92:	9c56                	add	s8,s8,s5
1c008d94:	02e00b93          	li	s7,46
1c008d98:	4aa5                	li	s5,9
1c008d9a:	a019                	j	1c008da0 <pos_libc_prf+0x1c4>
1c008d9c:	000dc703          	lbu	a4,0(s11)
1c008da0:	fd070793          	addi	a5,a4,-48
1c008da4:	853a                	mv	a0,a4
1c008da6:	85a6                	mv	a1,s1
1c008da8:	41bc06b3          	sub	a3,s8,s11
1c008dac:	00faf463          	bgeu	s5,a5,1c008db4 <pos_libc_prf+0x1d8>
1c008db0:	5f771ee3          	bne	a4,s7,1c009bac <pos_libc_prf+0xfd0>
1c008db4:	0d85                	addi	s11,s11,1
1c008db6:	9a02                	jalr	s4
1c008db8:	fff532e3          	p.bneimm	a0,-1,1c008d9c <pos_libc_prf+0x1c0>
1c008dbc:	5b7d                	li	s6,-1
1c008dbe:	bde1                	j	1c008c96 <pos_libc_prf+0xba>
1c008dc0:	0905                	addi	s2,s2,1
1c008dc2:	00094683          	lbu	a3,0(s2)
1c008dc6:	002d9793          	slli	a5,s11,0x2
1c008dca:	97ee                	add	a5,a5,s11
1c008dcc:	0786                	slli	a5,a5,0x1
1c008dce:	97de                	add	a5,a5,s7
1c008dd0:	fd068713          	addi	a4,a3,-48
1c008dd4:	886a                	mv	a6,s10
1c008dd6:	fd078d93          	addi	s11,a5,-48
1c008dda:	8d4a                	mv	s10,s2
1c008ddc:	8bb6                	mv	s7,a3
1c008dde:	fee571e3          	bgeu	a0,a4,1c008dc0 <pos_libc_prf+0x1e4>
1c008de2:	00280c93          	addi	s9,a6,2
1c008de6:	b739                	j	1c008cf4 <pos_libc_prf+0x118>
1c008de8:	47e2                	lw	a5,24(sp)
1c008dea:	0047ad8b          	p.lw	s11,4(a5!)
1c008dee:	000dd663          	bgez	s11,1c008dfa <pos_libc_prf+0x21e>
1c008df2:	4705                	li	a4,1
1c008df4:	41b00db3          	neg	s11,s11
1c008df8:	c63a                	sw	a4,12(sp)
1c008dfa:	00094683          	lbu	a3,0(s2)
1c008dfe:	cc3e                	sw	a5,24(sp)
1c008e00:	002d0c93          	addi	s9,s10,2
1c008e04:	bdc5                	j	1c008cf4 <pos_libc_prf+0x118>
1c008e06:	06300713          	li	a4,99
1c008e0a:	0ae69463          	bne	a3,a4,1c008eb2 <pos_libc_prf+0x2d6>
1c008e0e:	4d62                	lw	s10,24(sp)
1c008e10:	fffd8913          	addi	s2,s11,-1
1c008e14:	ca02                	sw	zero,20(sp)
1c008e16:	004d278b          	p.lw	a5,4(s10!)
1c008e1a:	c802                	sw	zero,16(sp)
1c008e1c:	4985                	li	s3,1
1c008e1e:	02f10a23          	sb	a5,52(sp)
1c008e22:	03410c13          	addi	s8,sp,52
1c008e26:	4401                	li	s0,0
1c008e28:	d402                	sw	zero,40(sp)
1c008e2a:	cc02                	sw	zero,24(sp)
1c008e2c:	47b2                	lw	a5,12(sp)
1c008e2e:	e385                	bnez	a5,1c008e4e <pos_libc_prf+0x272>
1c008e30:	01205f63          	blez	s2,1c008e4e <pos_libc_prf+0x272>
1c008e34:	fff90a93          	addi	s5,s2,-1
1c008e38:	85a6                	mv	a1,s1
1c008e3a:	02000513          	li	a0,32
1c008e3e:	9a02                	jalr	s4
1c008e40:	1afd                	addi	s5,s5,-1
1c008e42:	f7f52de3          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008e46:	fffab9e3          	p.bneimm	s5,-1,1c008e38 <pos_libc_prf+0x25c>
1c008e4a:	9b4a                	add	s6,s6,s2
1c008e4c:	597d                	li	s2,-1
1c008e4e:	47c2                	lw	a5,16(sp)
1c008e50:	00fc0ab3          	add	s5,s8,a5
1c008e54:	a031                	j	1c008e60 <pos_libc_prf+0x284>
1c008e56:	001c450b          	p.lbu	a0,1(s8!)
1c008e5a:	9a02                	jalr	s4
1c008e5c:	f7f520e3          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008e60:	85a6                	mv	a1,s1
1c008e62:	ff5c1ae3          	bne	s8,s5,1c008e56 <pos_libc_prf+0x27a>
1c008e66:	4bd2                	lw	s7,20(sp)
1c008e68:	a021                	j	1c008e70 <pos_libc_prf+0x294>
1c008e6a:	9a02                	jalr	s4
1c008e6c:	f5f528e3          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c008e70:	1bfd                	addi	s7,s7,-1
1c008e72:	85a6                	mv	a1,s1
1c008e74:	03000513          	li	a0,48
1c008e78:	fffbb9e3          	p.bneimm	s7,-1,1c008e6a <pos_libc_prf+0x28e>
1c008e7c:	47c2                	lw	a5,16(sp)
1c008e7e:	40f989b3          	sub	s3,s3,a5
1c008e82:	47e2                	lw	a5,24(sp)
1c008e84:	8c4e                	mv	s8,s3
1c008e86:	640792e3          	bnez	a5,1c009cca <pos_libc_prf+0x10ee>
1c008e8a:	57a2                	lw	a5,40(sp)
1c008e8c:	ee078ee3          	beqz	a5,1c008d88 <pos_libc_prf+0x1ac>
1c008e90:	9c56                	add	s8,s8,s5
1c008e92:	02e00d93          	li	s11,46
1c008e96:	a029                	j	1c008ea0 <pos_libc_prf+0x2c4>
1c008e98:	415c07b3          	sub	a5,s8,s5
1c008e9c:	edbb86e3          	beq	s7,s11,1c008d68 <pos_libc_prf+0x18c>
1c008ea0:	001acb8b          	p.lbu	s7,1(s5!)
1c008ea4:	85a6                	mv	a1,s1
1c008ea6:	855e                	mv	a0,s7
1c008ea8:	9a02                	jalr	s4
1c008eaa:	fff537e3          	p.bneimm	a0,-1,1c008e98 <pos_libc_prf+0x2bc>
1c008eae:	5b7d                	li	s6,-1
1c008eb0:	b3dd                	j	1c008c96 <pos_libc_prf+0xba>
1c008eb2:	1af69b63          	bne	a3,a5,1c009068 <pos_libc_prf+0x48c>
1c008eb6:	06c00793          	li	a5,108
1c008eba:	00f60c63          	beq	a2,a5,1c008ed2 <pos_libc_prf+0x2f6>
1c008ebe:	07a00793          	li	a5,122
1c008ec2:	00f60863          	beq	a2,a5,1c008ed2 <pos_libc_prf+0x2f6>
1c008ec6:	04c00793          	li	a5,76
1c008eca:	00f61463          	bne	a2,a5,1c008ed2 <pos_libc_prf+0x2f6>
1c008ece:	78c0106f          	j	1c00a65a <pos_libc_prf+0x1a7e>
1c008ed2:	4d62                	lw	s10,24(sp)
1c008ed4:	004d278b          	p.lw	a5,4(s10!)
1c008ed8:	41f7d713          	srai	a4,a5,0x1f
1c008edc:	d43a                	sw	a4,40(sp)
1c008ede:	86ba                	mv	a3,a4
1c008ee0:	5a06cce3          	bltz	a3,1c009c98 <pos_libc_prf+0x10bc>
1c008ee4:	4752                	lw	a4,20(sp)
1c008ee6:	64070ae3          	beqz	a4,1c009d3a <pos_libc_prf+0x115e>
1c008eea:	02b00713          	li	a4,43
1c008eee:	02e10a23          	sb	a4,52(sp)
1c008ef2:	03510713          	addi	a4,sp,53
1c008ef6:	89b6                	mv	s3,a3
1c008ef8:	cc3a                	sw	a4,24(sp)
1c008efa:	893a                	mv	s2,a4
1c008efc:	8abe                	mv	s5,a5
1c008efe:	4629                	li	a2,10
1c008f00:	4681                	li	a3,0
1c008f02:	8556                	mv	a0,s5
1c008f04:	85ce                	mv	a1,s3
1c008f06:	cd6ff0ef          	jal	ra,1c0083dc <__umoddi3>
1c008f0a:	8baa                	mv	s7,a0
1c008f0c:	85ce                	mv	a1,s3
1c008f0e:	8556                	mv	a0,s5
1c008f10:	4629                	li	a2,10
1c008f12:	4681                	li	a3,0
1c008f14:	988ff0ef          	jal	ra,1c00809c <__udivdi3>
1c008f18:	030b8b93          	addi	s7,s7,48
1c008f1c:	87ce                	mv	a5,s3
1c008f1e:	864a                	mv	a2,s2
1c008f20:	017900ab          	p.sb	s7,1(s2!)
1c008f24:	8756                	mv	a4,s5
1c008f26:	89ae                	mv	s3,a1
1c008f28:	8aaa                	mv	s5,a0
1c008f2a:	fbf1                	bnez	a5,1c008efe <pos_libc_prf+0x322>
1c008f2c:	47a5                	li	a5,9
1c008f2e:	fce7e8e3          	bltu	a5,a4,1c008efe <pos_libc_prf+0x322>
1c008f32:	47e2                	lw	a5,24(sp)
1c008f34:	00090023          	sb	zero,0(s2)
1c008f38:	0ac7f863          	bgeu	a5,a2,1c008fe8 <pos_libc_prf+0x40c>
1c008f3c:	fff7c693          	not	a3,a5
1c008f40:	96b2                	add	a3,a3,a2
1c008f42:	883e                	mv	a6,a5
1c008f44:	0016d793          	srli	a5,a3,0x1
1c008f48:	00178e13          	addi	t3,a5,1
1c008f4c:	01c80733          	add	a4,a6,t3
1c008f50:	40f607b3          	sub	a5,a2,a5
1c008f54:	04f737b3          	p.sletu	a5,a4,a5
1c008f58:	05093733          	p.sletu	a4,s2,a6
1c008f5c:	8fd9                	or	a5,a5,a4
1c008f5e:	85c2                	mv	a1,a6
1c008f60:	8732                	mv	a4,a2
1c008f62:	e399                	bnez	a5,1c008f68 <pos_libc_prf+0x38c>
1c008f64:	1ef0106f          	j	1c00a952 <pos_libc_prf+0x1d76>
1c008f68:	0066b693          	sltiu	a3,a3,6
1c008f6c:	c299                	beqz	a3,1c008f72 <pos_libc_prf+0x396>
1c008f6e:	1e50106f          	j	1c00a952 <pos_libc_prf+0x1d76>
1c008f72:	c20e3533          	p.bclr	a0,t3,1,0
1c008f76:	ffd60693          	addi	a3,a2,-3 # fffffd <__l1_heap_size+0xfc001d>
1c008f7a:	9542                	add	a0,a0,a6
1c008f7c:	4198                	lw	a4,0(a1)
1c008f7e:	429c                	lw	a5,0(a3)
1c008f80:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c008f84:	fee6ae2b          	p.sw	a4,-4(a3!)
1c008f88:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c008f8c:	00f5a22b          	p.sw	a5,4(a1!)
1c008f90:	fea596e3          	bne	a1,a0,1c008f7c <pos_libc_prf+0x3a0>
1c008f94:	4762                	lw	a4,24(sp)
1c008f96:	c20e37b3          	p.bclr	a5,t3,1,0
1c008f9a:	8e1d                	sub	a2,a2,a5
1c008f9c:	00f70ab3          	add	s5,a4,a5
1c008fa0:	05c78463          	beq	a5,t3,1c008fe8 <pos_libc_prf+0x40c>
1c008fa4:	000ac703          	lbu	a4,0(s5)
1c008fa8:	00064683          	lbu	a3,0(a2)
1c008fac:	87b2                	mv	a5,a2
1c008fae:	fee78fab          	p.sb	a4,-1(a5!)
1c008fb2:	8756                	mv	a4,s5
1c008fb4:	00d700ab          	p.sb	a3,1(a4!)
1c008fb8:	02f77863          	bgeu	a4,a5,1c008fe8 <pos_libc_prf+0x40c>
1c008fbc:	001ac683          	lbu	a3,1(s5)
1c008fc0:	fff64703          	lbu	a4,-1(a2)
1c008fc4:	002a8793          	addi	a5,s5,2
1c008fc8:	fed60fa3          	sb	a3,-1(a2)
1c008fcc:	00ea80a3          	sb	a4,1(s5)
1c008fd0:	ffe60713          	addi	a4,a2,-2
1c008fd4:	00e7fa63          	bgeu	a5,a4,1c008fe8 <pos_libc_prf+0x40c>
1c008fd8:	002ac703          	lbu	a4,2(s5)
1c008fdc:	ffe64783          	lbu	a5,-2(a2)
1c008fe0:	fee60f23          	sb	a4,-2(a2)
1c008fe4:	00fa8123          	sb	a5,2(s5)
1c008fe8:	185c                	addi	a5,sp,52
1c008fea:	40f909b3          	sub	s3,s2,a5
1c008fee:	47d2                	lw	a5,20(sp)
1c008ff0:	3a079ae3          	bnez	a5,1c009ba4 <pos_libc_prf+0xfc8>
1c008ff4:	47c2                	lw	a5,16(sp)
1c008ff6:	3a0797e3          	bnez	a5,1c009ba4 <pos_libc_prf+0xfc8>
1c008ffa:	57a2                	lw	a5,40(sp)
1c008ffc:	83fd                	srli	a5,a5,0x1f
1c008ffe:	c83e                	sw	a5,16(sp)
1c009000:	0c045ce3          	bgez	s0,1c0098d8 <pos_libc_prf+0xcfc>
1c009004:	413d8933          	sub	s2,s11,s3
1c009008:	4401                	li	s0,0
1c00900a:	d402                	sw	zero,40(sp)
1c00900c:	cc02                	sw	zero,24(sp)
1c00900e:	0c0c00e3          	beqz	s8,1c0098ce <pos_libc_prf+0xcf2>
1c009012:	040967b3          	p.max	a5,s2,zero
1c009016:	ca3e                	sw	a5,20(sp)
1c009018:	97ce                	add	a5,a5,s3
1c00901a:	40fd8933          	sub	s2,s11,a5
1c00901e:	03410c13          	addi	s8,sp,52
1c009022:	b529                	j	1c008e2c <pos_libc_prf+0x250>
1c009024:	07000793          	li	a5,112
1c009028:	24f683e3          	beq	a3,a5,1c009a6e <pos_libc_prf+0xe92>
1c00902c:	1cd7f063          	bgeu	a5,a3,1c0091ec <pos_libc_prf+0x610>
1c009030:	07300793          	li	a5,115
1c009034:	18f69963          	bne	a3,a5,1c0091c6 <pos_libc_prf+0x5ea>
1c009038:	4d62                	lw	s10,24(sp)
1c00903a:	004d2c0b          	p.lw	s8,4(s10!)
1c00903e:	24044f63          	bltz	s0,1c00929c <pos_libc_prf+0x6c0>
1c009042:	e019                	bnez	s0,1c009048 <pos_libc_prf+0x46c>
1c009044:	1610106f          	j	1c00a9a4 <pos_libc_prf+0x1dc8>
1c009048:	4705                	li	a4,1
1c00904a:	87e2                	mv	a5,s8
1c00904c:	4981                	li	s3,0
1c00904e:	04e46433          	p.max	s0,s0,a4
1c009052:	005440fb          	lp.setup	x1,s0,1c00905c <pos_libc_prf+0x480>
1c009056:	0017c70b          	p.lbu	a4,1(a5!)
1c00905a:	c311                	beqz	a4,1c00905e <pos_libc_prf+0x482>
1c00905c:	0985                	addi	s3,s3,1
1c00905e:	413d8933          	sub	s2,s11,s3
1c009062:	ca02                	sw	zero,20(sp)
1c009064:	c802                	sw	zero,16(sp)
1c009066:	b3c1                	j	1c008e26 <pos_libc_prf+0x24a>
1c009068:	05800793          	li	a5,88
1c00906c:	16f69163          	bne	a3,a5,1c0091ce <pos_libc_prf+0x5f2>
1c009070:	06c00793          	li	a5,108
1c009074:	00f60c63          	beq	a2,a5,1c00908c <pos_libc_prf+0x4b0>
1c009078:	07a00793          	li	a5,122
1c00907c:	00f60863          	beq	a2,a5,1c00908c <pos_libc_prf+0x4b0>
1c009080:	04c00793          	li	a5,76
1c009084:	00f61463          	bne	a2,a5,1c00908c <pos_libc_prf+0x4b0>
1c009088:	5ec0106f          	j	1c00a674 <pos_libc_prf+0x1a98>
1c00908c:	4d62                	lw	s10,24(sp)
1c00908e:	4a81                	li	s5,0
1c009090:	004d280b          	p.lw	a6,4(s10!)
1c009094:	06f00793          	li	a5,111
1c009098:	00f69463          	bne	a3,a5,1c0090a0 <pos_libc_prf+0x4c4>
1c00909c:	7800106f          	j	1c00a81c <pos_libc_prf+0x1c40>
1c0090a0:	07500793          	li	a5,117
1c0090a4:	00f69463          	bne	a3,a5,1c0090ac <pos_libc_prf+0x4d0>
1c0090a8:	5ea0106f          	j	1c00a692 <pos_libc_prf+0x1ab6>
1c0090ac:	57b2                	lw	a5,44(sp)
1c0090ae:	1848                	addi	a0,sp,52
1c0090b0:	c791                	beqz	a5,1c0090bc <pos_libc_prf+0x4e0>
1c0090b2:	47f2                	lw	a5,28(sp)
1c0090b4:	03610513          	addi	a0,sp,54
1c0090b8:	02f11a23          	sh	a5,52(sp)
1c0090bc:	85aa                	mv	a1,a0
1c0090be:	4ea5                	li	t4,9
1c0090c0:	4f3d                	li	t5,15
1c0090c2:	a801                	j	1c0090d2 <pos_libc_prf+0x4f6>
1c0090c4:	006980ab          	p.sb	t1,1(s3!)
1c0090c8:	020a8963          	beqz	s5,1c0090fa <pos_libc_prf+0x51e>
1c0090cc:	8832                	mv	a6,a2
1c0090ce:	8aba                	mv	s5,a4
1c0090d0:	85ce                	mv	a1,s3
1c0090d2:	f64837b3          	p.bclr	a5,a6,27,4
1c0090d6:	01ca9713          	slli	a4,s5,0x1c
1c0090da:	00485613          	srli	a2,a6,0x4
1c0090de:	8e59                	or	a2,a2,a4
1c0090e0:	89ae                	mv	s3,a1
1c0090e2:	03078313          	addi	t1,a5,48
1c0090e6:	05778893          	addi	a7,a5,87
1c0090ea:	004ad713          	srli	a4,s5,0x4
1c0090ee:	fcfefbe3          	bgeu	t4,a5,1c0090c4 <pos_libc_prf+0x4e8>
1c0090f2:	011980ab          	p.sb	a7,1(s3!)
1c0090f6:	fc0a9be3          	bnez	s5,1c0090cc <pos_libc_prf+0x4f0>
1c0090fa:	fd0f69e3          	bltu	t5,a6,1c0090cc <pos_libc_prf+0x4f0>
1c0090fe:	00098023          	sb	zero,0(s3)
1c009102:	0ab57563          	bgeu	a0,a1,1c0091ac <pos_libc_prf+0x5d0>
1c009106:	fff54313          	not	t1,a0
1c00910a:	932e                	add	t1,t1,a1
1c00910c:	00135793          	srli	a5,t1,0x1
1c009110:	00178e13          	addi	t3,a5,1
1c009114:	40f58733          	sub	a4,a1,a5
1c009118:	01c507b3          	add	a5,a0,t3
1c00911c:	04e7b733          	p.sletu	a4,a5,a4
1c009120:	04a9b7b3          	p.sletu	a5,s3,a0
1c009124:	8f5d                	or	a4,a4,a5
1c009126:	862a                	mv	a2,a0
1c009128:	87ae                	mv	a5,a1
1c00912a:	16070e63          	beqz	a4,1c0092a6 <pos_libc_prf+0x6ca>
1c00912e:	00633313          	sltiu	t1,t1,6
1c009132:	16031a63          	bnez	t1,1c0092a6 <pos_libc_prf+0x6ca>
1c009136:	c20e3eb3          	p.bclr	t4,t3,1,0
1c00913a:	ffd58313          	addi	t1,a1,-3
1c00913e:	9eaa                	add	t4,t4,a0
1c009140:	4218                	lw	a4,0(a2)
1c009142:	00032783          	lw	a5,0(t1)
1c009146:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c00914a:	fee32e2b          	p.sw	a4,-4(t1!)
1c00914e:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c009152:	00f6222b          	p.sw	a5,4(a2!)
1c009156:	fece95e3          	bne	t4,a2,1c009140 <pos_libc_prf+0x564>
1c00915a:	c20e3733          	p.bclr	a4,t3,1,0
1c00915e:	8d99                	sub	a1,a1,a4
1c009160:	00e507b3          	add	a5,a0,a4
1c009164:	05c70463          	beq	a4,t3,1c0091ac <pos_libc_prf+0x5d0>
1c009168:	0007c603          	lbu	a2,0(a5)
1c00916c:	0005c503          	lbu	a0,0(a1)
1c009170:	872e                	mv	a4,a1
1c009172:	fec70fab          	p.sb	a2,-1(a4!)
1c009176:	863e                	mv	a2,a5
1c009178:	00a600ab          	p.sb	a0,1(a2!)
1c00917c:	02e67863          	bgeu	a2,a4,1c0091ac <pos_libc_prf+0x5d0>
1c009180:	0017c503          	lbu	a0,1(a5)
1c009184:	fff5c603          	lbu	a2,-1(a1)
1c009188:	00278713          	addi	a4,a5,2
1c00918c:	fea58fa3          	sb	a0,-1(a1)
1c009190:	00c780a3          	sb	a2,1(a5)
1c009194:	ffe58613          	addi	a2,a1,-2
1c009198:	00c77a63          	bgeu	a4,a2,1c0091ac <pos_libc_prf+0x5d0>
1c00919c:	0027c603          	lbu	a2,2(a5)
1c0091a0:	ffe5c703          	lbu	a4,-2(a1)
1c0091a4:	fec58f23          	sb	a2,-2(a1)
1c0091a8:	00e78123          	sb	a4,2(a5)
1c0091ac:	05800793          	li	a5,88
1c0091b0:	00f69463          	bne	a3,a5,1c0091b8 <pos_libc_prf+0x5dc>
1c0091b4:	61c0106f          	j	1c00a7d0 <pos_libc_prf+0x1bf4>
1c0091b8:	185c                	addi	a5,sp,52
1c0091ba:	40f989b3          	sub	s3,s3,a5
1c0091be:	57b2                	lw	a5,44(sp)
1c0091c0:	0786                	slli	a5,a5,0x1
1c0091c2:	c83e                	sw	a5,16(sp)
1c0091c4:	bd35                	j	1c009000 <pos_libc_prf+0x424>
1c0091c6:	07200793          	li	a5,114
1c0091ca:	04d7ef63          	bltu	a5,a3,1c009228 <pos_libc_prf+0x64c>
1c0091ce:	85a6                	mv	a1,s1
1c0091d0:	02500513          	li	a0,37
1c0091d4:	c636                	sw	a3,12(sp)
1c0091d6:	9a02                	jalr	s4
1c0091d8:	bff522e3          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c0091dc:	46b2                	lw	a3,12(sp)
1c0091de:	85a6                	mv	a1,s1
1c0091e0:	8536                	mv	a0,a3
1c0091e2:	9a02                	jalr	s4
1c0091e4:	bdf52ce3          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c0091e8:	0b09                	addi	s6,s6,2
1c0091ea:	b4b1                	j	1c008c36 <pos_libc_prf+0x5a>
1c0091ec:	06e00793          	li	a5,110
1c0091f0:	04f69563          	bne	a3,a5,1c00923a <pos_libc_prf+0x65e>
1c0091f4:	46e2                	lw	a3,24(sp)
1c0091f6:	06800793          	li	a5,104
1c0091fa:	0046a70b          	p.lw	a4,4(a3!)
1c0091fe:	cc36                	sw	a3,24(sp)
1c009200:	0af60e63          	beq	a2,a5,1c0092bc <pos_libc_prf+0x6e0>
1c009204:	18c7cce3          	blt	a5,a2,1c009b9c <pos_libc_prf+0xfc0>
1c009208:	04800793          	li	a5,72
1c00920c:	00f61463          	bne	a2,a5,1c009214 <pos_libc_prf+0x638>
1c009210:	73a0106f          	j	1c00a94a <pos_libc_prf+0x1d6e>
1c009214:	04c00793          	li	a5,76
1c009218:	18f612e3          	bne	a2,a5,1c009b9c <pos_libc_prf+0xfc0>
1c00921c:	41fb5793          	srai	a5,s6,0x1f
1c009220:	01672023          	sw	s6,0(a4)
1c009224:	c35c                	sw	a5,4(a4)
1c009226:	bc01                	j	1c008c36 <pos_libc_prf+0x5a>
1c009228:	07500793          	li	a5,117
1c00922c:	e4f682e3          	beq	a3,a5,1c009070 <pos_libc_prf+0x494>
1c009230:	07800793          	li	a5,120
1c009234:	e2f68ee3          	beq	a3,a5,1c009070 <pos_libc_prf+0x494>
1c009238:	bf59                	j	1c0091ce <pos_libc_prf+0x5f2>
1c00923a:	06f00793          	li	a5,111
1c00923e:	e2f689e3          	beq	a3,a5,1c009070 <pos_libc_prf+0x494>
1c009242:	06900793          	li	a5,105
1c009246:	c6f688e3          	beq	a3,a5,1c008eb6 <pos_libc_prf+0x2da>
1c00924a:	b751                	j	1c0091ce <pos_libc_prf+0x5f2>
1c00924c:	87e6                	mv	a5,s9
1c00924e:	0017c68b          	p.lbu	a3,1(a5!)
1c009252:	02a00713          	li	a4,42
1c009256:	06e69663          	bne	a3,a4,1c0092c2 <pos_libc_prf+0x6e6>
1c00925a:	47e2                	lw	a5,24(sp)
1c00925c:	001cc683          	lbu	a3,1(s9)
1c009260:	0c89                	addi	s9,s9,2
1c009262:	0047a40b          	p.lw	s0,4(a5!)
1c009266:	89b6                	mv	s3,a3
1c009268:	cc3e                	sw	a5,24(sp)
1c00926a:	bc59                	j	1c008d00 <pos_libc_prf+0x124>
1c00926c:	01368c63          	beq	a3,s3,1c009284 <pos_libc_prf+0x6a8>
1c009270:	8cbe                	mv	s9,a5
1c009272:	06c00613          	li	a2,108
1c009276:	bc65                	j	1c008d2e <pos_libc_prf+0x152>
1c009278:	01368c63          	beq	a3,s3,1c009290 <pos_libc_prf+0x6b4>
1c00927c:	8cbe                	mv	s9,a5
1c00927e:	06800613          	li	a2,104
1c009282:	b475                	j	1c008d2e <pos_libc_prf+0x152>
1c009284:	001cc683          	lbu	a3,1(s9)
1c009288:	04c00613          	li	a2,76
1c00928c:	0c89                	addi	s9,s9,2
1c00928e:	b445                	j	1c008d2e <pos_libc_prf+0x152>
1c009290:	001cc683          	lbu	a3,1(s9)
1c009294:	04800613          	li	a2,72
1c009298:	0c89                	addi	s9,s9,2
1c00929a:	bc51                	j	1c008d2e <pos_libc_prf+0x152>
1c00929c:	80000437          	lui	s0,0x80000
1c0092a0:	fff44413          	not	s0,s0
1c0092a4:	b355                	j	1c009048 <pos_libc_prf+0x46c>
1c0092a6:	00054603          	lbu	a2,0(a0)
1c0092aa:	0007c703          	lbu	a4,0(a5)
1c0092ae:	fec78fab          	p.sb	a2,-1(a5!)
1c0092b2:	00e500ab          	p.sb	a4,1(a0!)
1c0092b6:	fef568e3          	bltu	a0,a5,1c0092a6 <pos_libc_prf+0x6ca>
1c0092ba:	bdcd                	j	1c0091ac <pos_libc_prf+0x5d0>
1c0092bc:	01671023          	sh	s6,0(a4)
1c0092c0:	ba9d                	j	1c008c36 <pos_libc_prf+0x5a>
1c0092c2:	fd068593          	addi	a1,a3,-48
1c0092c6:	4525                	li	a0,9
1c0092c8:	89b6                	mv	s3,a3
1c0092ca:	873e                	mv	a4,a5
1c0092cc:	4401                	li	s0,0
1c0092ce:	4625                	li	a2,9
1c0092d0:	8cbe                	mv	s9,a5
1c0092d2:	a2b567e3          	bltu	a0,a1,1c008d00 <pos_libc_prf+0x124>
1c0092d6:	00241793          	slli	a5,s0,0x2
1c0092da:	97a2                	add	a5,a5,s0
1c0092dc:	0786                	slli	a5,a5,0x1
1c0092de:	97b6                	add	a5,a5,a3
1c0092e0:	0017468b          	p.lbu	a3,1(a4!)
1c0092e4:	fd078413          	addi	s0,a5,-48
1c0092e8:	fd068793          	addi	a5,a3,-48
1c0092ec:	89b6                	mv	s3,a3
1c0092ee:	fef674e3          	bgeu	a2,a5,1c0092d6 <pos_libc_prf+0x6fa>
1c0092f2:	8cba                	mv	s9,a4
1c0092f4:	b431                	j	1c008d00 <pos_libc_prf+0x124>
1c0092f6:	4501                	li	a0,0
1c0092f8:	4601                	li	a2,0
1c0092fa:	4291                	li	t0,4
1c0092fc:	40c282b3          	sub	t0,t0,a2
1c009300:	0132c0fb          	lp.setup	x1,t0,1c009326 <pos_libc_prf+0x74a>
1c009304:	01ff1713          	slli	a4,t5,0x1f
1c009308:	001ed793          	srli	a5,t4,0x1
1c00930c:	8fd9                	or	a5,a5,a4
1c00930e:	fc1ebeb3          	p.bclr	t4,t4,30,1
1c009312:	00fe8e33          	add	t3,t4,a5
1c009316:	001f5f13          	srli	t5,t5,0x1
1c00931a:	01de3eb3          	sltu	t4,t3,t4
1c00931e:	01ee8733          	add	a4,t4,t5
1c009322:	8f3a                	mv	t5,a4
1c009324:	8ef2                	mv	t4,t3
1c009326:	0605                	addi	a2,a2,1
1c009328:	00045363          	bgez	s0,1c00932e <pos_libc_prf+0x752>
1c00932c:	4419                	li	s0,6
1c00932e:	04700793          	li	a5,71
1c009332:	0cff8fe3          	beq	t6,a5,1c009c10 <pos_libc_prf+0x1034>
1c009336:	06600793          	li	a5,102
1c00933a:	00f69463          	bne	a3,a5,1c009342 <pos_libc_prf+0x766>
1c00933e:	6520106f          	j	1c00a990 <pos_libc_prf+0x1db4>
1c009342:	00140e93          	addi	t4,s0,1 # 80000001 <__l2_shared_end+0x63fefbf9>
1c009346:	47c1                	li	a5,16
1c009348:	04feceb3          	p.min	t4,t4,a5
1c00934c:	1efd                	addi	t4,t4,-1
1c00934e:	4581                	li	a1,0
1c009350:	4781                	li	a5,0
1c009352:	080002b7          	lui	t0,0x8000
1c009356:	4f15                	li	t5,5
1c009358:	0e85                	addi	t4,t4,1
1c00935a:	032ec0fb          	lp.setup	x1,t4,1c0093be <pos_libc_prf+0x7e2>
1c00935e:	00278813          	addi	a6,a5,2
1c009362:	00f837b3          	sltu	a5,a6,a5
1c009366:	9796                	add	a5,a5,t0
1c009368:	03e7d2b3          	divu	t0,a5,t5
1c00936c:	00385f93          	srli	t6,a6,0x3
1c009370:	00229893          	slli	a7,t0,0x2
1c009374:	9896                	add	a7,a7,t0
1c009376:	411787b3          	sub	a5,a5,a7
1c00937a:	07f6                	slli	a5,a5,0x1d
1c00937c:	01f7efb3          	or	t6,a5,t6
1c009380:	03efdfb3          	divu	t6,t6,t5
1c009384:	003f9893          	slli	a7,t6,0x3
1c009388:	005f9793          	slli	a5,t6,0x5
1c00938c:	97c6                	add	a5,a5,a7
1c00938e:	40f80833          	sub	a6,a6,a5
1c009392:	03e857b3          	divu	a5,a6,t5
1c009396:	01dfdf93          	srli	t6,t6,0x1d
1c00939a:	9f96                	add	t6,t6,t0
1c00939c:	98be                	add	a7,a7,a5
1c00939e:	00f8b833          	sltu	a6,a7,a5
1c0093a2:	987e                	add	a6,a6,t6
1c0093a4:	01f81f93          	slli	t6,a6,0x1f
1c0093a8:	0018d793          	srli	a5,a7,0x1
1c0093ac:	00ffe7b3          	or	a5,t6,a5
1c0093b0:	fc18b8b3          	p.bclr	a7,a7,30,1
1c0093b4:	97c6                	add	a5,a5,a7
1c0093b6:	00185813          	srli	a6,a6,0x1
1c0093ba:	0117b8b3          	sltu	a7,a5,a7
1c0093be:	010882b3          	add	t0,a7,a6
1c0093c2:	97f2                	add	a5,a5,t3
1c0093c4:	9716                	add	a4,a4,t0
1c0093c6:	01c7be33          	sltu	t3,a5,t3
1c0093ca:	00ee02b3          	add	t0,t3,a4
1c0093ce:	f602b733          	p.bclr	a4,t0,27,0
1c0093d2:	c725                	beqz	a4,1c00943a <pos_libc_prf+0x85e>
1c0093d4:	00278e93          	addi	t4,a5,2
1c0093d8:	00feb733          	sltu	a4,t4,a5
1c0093dc:	92ba                	add	t0,t0,a4
1c0093de:	4f95                	li	t6,5
1c0093e0:	03f2df33          	divu	t5,t0,t6
1c0093e4:	003ed713          	srli	a4,t4,0x3
1c0093e8:	0505                	addi	a0,a0,1
1c0093ea:	002f1e13          	slli	t3,t5,0x2
1c0093ee:	9e7a                	add	t3,t3,t5
1c0093f0:	41c282b3          	sub	t0,t0,t3
1c0093f4:	01d29e13          	slli	t3,t0,0x1d
1c0093f8:	00ee6733          	or	a4,t3,a4
1c0093fc:	03f75733          	divu	a4,a4,t6
1c009400:	00371e13          	slli	t3,a4,0x3
1c009404:	00571793          	slli	a5,a4,0x5
1c009408:	97f2                	add	a5,a5,t3
1c00940a:	40fe87b3          	sub	a5,t4,a5
1c00940e:	03f7d7b3          	divu	a5,a5,t6
1c009412:	8375                	srli	a4,a4,0x1d
1c009414:	977a                	add	a4,a4,t5
1c009416:	9e3e                	add	t3,t3,a5
1c009418:	00fe37b3          	sltu	a5,t3,a5
1c00941c:	973e                	add	a4,a4,a5
1c00941e:	01f71e93          	slli	t4,a4,0x1f
1c009422:	001e5793          	srli	a5,t3,0x1
1c009426:	00fee7b3          	or	a5,t4,a5
1c00942a:	fc1e3e33          	p.bclr	t3,t3,30,1
1c00942e:	97f2                	add	a5,a5,t3
1c009430:	8305                	srli	a4,a4,0x1
1c009432:	01c7be33          	sltu	t3,a5,t3
1c009436:	00ee02b3          	add	t0,t3,a4
1c00943a:	06600e13          	li	t3,102
1c00943e:	00130713          	addi	a4,t1,1
1c009442:	13c686e3          	beq	a3,t3,1c009d6e <pos_libc_prf+0x1192>
1c009446:	01e7de93          	srli	t4,a5,0x1e
1c00944a:	00279613          	slli	a2,a5,0x2
1c00944e:	00229e13          	slli	t3,t0,0x2
1c009452:	97b2                	add	a5,a5,a2
1c009454:	01ceee33          	or	t3,t4,t3
1c009458:	00c7b633          	sltu	a2,a5,a2
1c00945c:	9e16                	add	t3,t3,t0
1c00945e:	9672                	add	a2,a2,t3
1c009460:	0606                	slli	a2,a2,0x1
1c009462:	01f7de13          	srli	t3,a5,0x1f
1c009466:	00ce6633          	or	a2,t3,a2
1c00946a:	01c65e13          	srli	t3,a2,0x1c
1c00946e:	c7c63eb3          	p.bclr	t4,a2,3,28
1c009472:	5632                	lw	a2,44(sp)
1c009474:	8f72                	mv	t5,t3
1c009476:	030e0e13          	addi	t3,t3,48 # 10030 <pos_soc_event_callback+0xfbec>
1c00947a:	01e03f33          	snez	t5,t5
1c00947e:	01c30023          	sb	t3,0(t1)
1c009482:	0786                	slli	a5,a5,0x1
1c009484:	41e50533          	sub	a0,a0,t5
1c009488:	08061fe3          	bnez	a2,1c009d26 <pos_libc_prf+0x114a>
1c00948c:	3c040e63          	beqz	s0,1c009868 <pos_libc_prf+0xc8c>
1c009490:	02e00713          	li	a4,46
1c009494:	00230613          	addi	a2,t1,2
1c009498:	00e300a3          	sb	a4,1(t1)
1c00949c:	00279713          	slli	a4,a5,0x2
1c0094a0:	01e7de13          	srli	t3,a5,0x1e
1c0094a4:	002e9313          	slli	t1,t4,0x2
1c0094a8:	97ba                	add	a5,a5,a4
1c0094aa:	006e6333          	or	t1,t3,t1
1c0094ae:	9376                	add	t1,t1,t4
1c0094b0:	00e7be33          	sltu	t3,a5,a4
1c0094b4:	9e1a                	add	t3,t3,t1
1c0094b6:	01f7d713          	srli	a4,a5,0x1f
1c0094ba:	0e06                	slli	t3,t3,0x1
1c0094bc:	01c76e33          	or	t3,a4,t3
1c0094c0:	01ce5713          	srli	a4,t3,0x1c
1c0094c4:	03070713          	addi	a4,a4,48
1c0094c8:	10000337          	lui	t1,0x10000
1c0094cc:	137d                	addi	t1,t1,-1
1c0094ce:	00e60023          	sb	a4,0(a2)
1c0094d2:	006e7e33          	and	t3,t3,t1
1c0094d6:	00179713          	slli	a4,a5,0x1
1c0094da:	00143463          	p.bneimm	s0,1,1c0094e2 <pos_libc_prf+0x906>
1c0094de:	6240106f          	j	1c00ab02 <pos_libc_prf+0x1f26>
1c0094e2:	078e                	slli	a5,a5,0x3
1c0094e4:	01e75f13          	srli	t5,a4,0x1e
1c0094e8:	002e1e93          	slli	t4,t3,0x2
1c0094ec:	973e                	add	a4,a4,a5
1c0094ee:	01df6eb3          	or	t4,t5,t4
1c0094f2:	9e76                	add	t3,t3,t4
1c0094f4:	00f737b3          	sltu	a5,a4,a5
1c0094f8:	97f2                	add	a5,a5,t3
1c0094fa:	0786                	slli	a5,a5,0x1
1c0094fc:	01f75e13          	srli	t3,a4,0x1f
1c009500:	00fe67b3          	or	a5,t3,a5
1c009504:	01c7de13          	srli	t3,a5,0x1c
1c009508:	030e0e13          	addi	t3,t3,48
1c00950c:	01c600a3          	sb	t3,1(a2)
1c009510:	00171e93          	slli	t4,a4,0x1
1c009514:	0067f7b3          	and	a5,a5,t1
1c009518:	00243463          	p.bneimm	s0,2,1c009520 <pos_libc_prf+0x944>
1c00951c:	5dc0106f          	j	1c00aaf8 <pos_libc_prf+0x1f1c>
1c009520:	01eedf13          	srli	t5,t4,0x1e
1c009524:	070e                	slli	a4,a4,0x3
1c009526:	00279e13          	slli	t3,a5,0x2
1c00952a:	9eba                	add	t4,t4,a4
1c00952c:	01cf6e33          	or	t3,t5,t3
1c009530:	00eeb733          	sltu	a4,t4,a4
1c009534:	97f2                	add	a5,a5,t3
1c009536:	97ba                	add	a5,a5,a4
1c009538:	0786                	slli	a5,a5,0x1
1c00953a:	01fed713          	srli	a4,t4,0x1f
1c00953e:	8fd9                	or	a5,a5,a4
1c009540:	01c7de13          	srli	t3,a5,0x1c
1c009544:	030e0e13          	addi	t3,t3,48
1c009548:	01c60123          	sb	t3,2(a2)
1c00954c:	ffd40f13          	addi	t5,s0,-3
1c009550:	001e9e13          	slli	t3,t4,0x1
1c009554:	0067f7b3          	and	a5,a5,t1
1c009558:	01e04463          	bgtz	t5,1c009560 <pos_libc_prf+0x984>
1c00955c:	5920106f          	j	1c00aaee <pos_libc_prf+0x1f12>
1c009560:	003e9713          	slli	a4,t4,0x3
1c009564:	01ee5f13          	srli	t5,t3,0x1e
1c009568:	00279e93          	slli	t4,a5,0x2
1c00956c:	9e3a                	add	t3,t3,a4
1c00956e:	01df6eb3          	or	t4,t5,t4
1c009572:	00ee3733          	sltu	a4,t3,a4
1c009576:	97f6                	add	a5,a5,t4
1c009578:	97ba                	add	a5,a5,a4
1c00957a:	0786                	slli	a5,a5,0x1
1c00957c:	01fe5713          	srli	a4,t3,0x1f
1c009580:	8fd9                	or	a5,a5,a4
1c009582:	01c7d713          	srli	a4,a5,0x1c
1c009586:	03070713          	addi	a4,a4,48
1c00958a:	00e601a3          	sb	a4,3(a2)
1c00958e:	ffc40e93          	addi	t4,s0,-4
1c009592:	001e1f13          	slli	t5,t3,0x1
1c009596:	0067f7b3          	and	a5,a5,t1
1c00959a:	01d04463          	bgtz	t4,1c0095a2 <pos_libc_prf+0x9c6>
1c00959e:	5460106f          	j	1c00aae4 <pos_libc_prf+0x1f08>
1c0095a2:	003e1713          	slli	a4,t3,0x3
1c0095a6:	01ef5e93          	srli	t4,t5,0x1e
1c0095aa:	00279e13          	slli	t3,a5,0x2
1c0095ae:	9f3a                	add	t5,t5,a4
1c0095b0:	01ceee33          	or	t3,t4,t3
1c0095b4:	97f2                	add	a5,a5,t3
1c0095b6:	00ef3733          	sltu	a4,t5,a4
1c0095ba:	973e                	add	a4,a4,a5
1c0095bc:	0706                	slli	a4,a4,0x1
1c0095be:	01ff5793          	srli	a5,t5,0x1f
1c0095c2:	8f5d                	or	a4,a4,a5
1c0095c4:	01c75e13          	srli	t3,a4,0x1c
1c0095c8:	030e0e13          	addi	t3,t3,48
1c0095cc:	01c60223          	sb	t3,4(a2)
1c0095d0:	ffb40793          	addi	a5,s0,-5
1c0095d4:	001f1e93          	slli	t4,t5,0x1
1c0095d8:	00677733          	and	a4,a4,t1
1c0095dc:	00f04463          	bgtz	a5,1c0095e4 <pos_libc_prf+0xa08>
1c0095e0:	4fa0106f          	j	1c00aada <pos_libc_prf+0x1efe>
1c0095e4:	003f1793          	slli	a5,t5,0x3
1c0095e8:	00271e13          	slli	t3,a4,0x2
1c0095ec:	01eedf13          	srli	t5,t4,0x1e
1c0095f0:	01cf6e33          	or	t3,t5,t3
1c0095f4:	9ebe                	add	t4,t4,a5
1c0095f6:	9772                	add	a4,a4,t3
1c0095f8:	00feb7b3          	sltu	a5,t4,a5
1c0095fc:	97ba                	add	a5,a5,a4
1c0095fe:	0786                	slli	a5,a5,0x1
1c009600:	01fed713          	srli	a4,t4,0x1f
1c009604:	8fd9                	or	a5,a5,a4
1c009606:	01c7de13          	srli	t3,a5,0x1c
1c00960a:	030e0e13          	addi	t3,t3,48
1c00960e:	01c602a3          	sb	t3,5(a2)
1c009612:	ffa40e13          	addi	t3,s0,-6
1c009616:	001e9f13          	slli	t5,t4,0x1
1c00961a:	0067f7b3          	and	a5,a5,t1
1c00961e:	01c04463          	bgtz	t3,1c009626 <pos_libc_prf+0xa4a>
1c009622:	4ae0106f          	j	1c00aad0 <pos_libc_prf+0x1ef4>
1c009626:	003e9713          	slli	a4,t4,0x3
1c00962a:	00279e13          	slli	t3,a5,0x2
1c00962e:	01ef5e93          	srli	t4,t5,0x1e
1c009632:	01ceee33          	or	t3,t4,t3
1c009636:	9f3a                	add	t5,t5,a4
1c009638:	97f2                	add	a5,a5,t3
1c00963a:	00ef3733          	sltu	a4,t5,a4
1c00963e:	973e                	add	a4,a4,a5
1c009640:	0706                	slli	a4,a4,0x1
1c009642:	01ff5793          	srli	a5,t5,0x1f
1c009646:	8f5d                	or	a4,a4,a5
1c009648:	01c75e13          	srli	t3,a4,0x1c
1c00964c:	030e0e13          	addi	t3,t3,48
1c009650:	01c60323          	sb	t3,6(a2)
1c009654:	ff940793          	addi	a5,s0,-7
1c009658:	001f1e93          	slli	t4,t5,0x1
1c00965c:	00677733          	and	a4,a4,t1
1c009660:	00f04463          	bgtz	a5,1c009668 <pos_libc_prf+0xa8c>
1c009664:	4620106f          	j	1c00aac6 <pos_libc_prf+0x1eea>
1c009668:	003f1793          	slli	a5,t5,0x3
1c00966c:	00271e13          	slli	t3,a4,0x2
1c009670:	01eedf13          	srli	t5,t4,0x1e
1c009674:	01cf6e33          	or	t3,t5,t3
1c009678:	9ebe                	add	t4,t4,a5
1c00967a:	9772                	add	a4,a4,t3
1c00967c:	00feb7b3          	sltu	a5,t4,a5
1c009680:	97ba                	add	a5,a5,a4
1c009682:	0786                	slli	a5,a5,0x1
1c009684:	01fed713          	srli	a4,t4,0x1f
1c009688:	8fd9                	or	a5,a5,a4
1c00968a:	01c7de13          	srli	t3,a5,0x1c
1c00968e:	030e0e13          	addi	t3,t3,48
1c009692:	01c603a3          	sb	t3,7(a2)
1c009696:	ff840e13          	addi	t3,s0,-8
1c00969a:	001e9f13          	slli	t5,t4,0x1
1c00969e:	0067f7b3          	and	a5,a5,t1
1c0096a2:	01c04463          	bgtz	t3,1c0096aa <pos_libc_prf+0xace>
1c0096a6:	4160106f          	j	1c00aabc <pos_libc_prf+0x1ee0>
1c0096aa:	003e9713          	slli	a4,t4,0x3
1c0096ae:	00279e13          	slli	t3,a5,0x2
1c0096b2:	01ef5e93          	srli	t4,t5,0x1e
1c0096b6:	01ceee33          	or	t3,t4,t3
1c0096ba:	9f3a                	add	t5,t5,a4
1c0096bc:	97f2                	add	a5,a5,t3
1c0096be:	00ef3733          	sltu	a4,t5,a4
1c0096c2:	973e                	add	a4,a4,a5
1c0096c4:	0706                	slli	a4,a4,0x1
1c0096c6:	01ff5793          	srli	a5,t5,0x1f
1c0096ca:	8f5d                	or	a4,a4,a5
1c0096cc:	01c75e13          	srli	t3,a4,0x1c
1c0096d0:	030e0e13          	addi	t3,t3,48
1c0096d4:	01c60423          	sb	t3,8(a2)
1c0096d8:	ff740793          	addi	a5,s0,-9
1c0096dc:	001f1e93          	slli	t4,t5,0x1
1c0096e0:	00677e33          	and	t3,a4,t1
1c0096e4:	00f04463          	bgtz	a5,1c0096ec <pos_libc_prf+0xb10>
1c0096e8:	4420106f          	j	1c00ab2a <pos_libc_prf+0x1f4e>
1c0096ec:	003f1793          	slli	a5,t5,0x3
1c0096f0:	01eed713          	srli	a4,t4,0x1e
1c0096f4:	002e1f13          	slli	t5,t3,0x2
1c0096f8:	9ebe                	add	t4,t4,a5
1c0096fa:	01e76f33          	or	t5,a4,t5
1c0096fe:	9e7a                	add	t3,t3,t5
1c009700:	00feb733          	sltu	a4,t4,a5
1c009704:	9772                	add	a4,a4,t3
1c009706:	01fed793          	srli	a5,t4,0x1f
1c00970a:	0706                	slli	a4,a4,0x1
1c00970c:	8f5d                	or	a4,a4,a5
1c00970e:	01c75793          	srli	a5,a4,0x1c
1c009712:	03078793          	addi	a5,a5,48
1c009716:	00f604a3          	sb	a5,9(a2)
1c00971a:	ff640e13          	addi	t3,s0,-10
1c00971e:	001e9793          	slli	a5,t4,0x1
1c009722:	00677733          	and	a4,a4,t1
1c009726:	01c04463          	bgtz	t3,1c00972e <pos_libc_prf+0xb52>
1c00972a:	3f60106f          	j	1c00ab20 <pos_libc_prf+0x1f44>
1c00972e:	00279313          	slli	t1,a5,0x2
1c009732:	01e7de93          	srli	t4,a5,0x1e
1c009736:	00271e13          	slli	t3,a4,0x2
1c00973a:	979a                	add	a5,a5,t1
1c00973c:	01ceee33          	or	t3,t4,t3
1c009740:	0067b333          	sltu	t1,a5,t1
1c009744:	9772                	add	a4,a4,t3
1c009746:	971a                	add	a4,a4,t1
1c009748:	0706                	slli	a4,a4,0x1
1c00974a:	01f7d313          	srli	t1,a5,0x1f
1c00974e:	00e36733          	or	a4,t1,a4
1c009752:	01c75e13          	srli	t3,a4,0x1c
1c009756:	030e0e13          	addi	t3,t3,48
1c00975a:	10000337          	lui	t1,0x10000
1c00975e:	137d                	addi	t1,t1,-1
1c009760:	01c60523          	sb	t3,10(a2)
1c009764:	ff540e13          	addi	t3,s0,-11
1c009768:	00179f13          	slli	t5,a5,0x1
1c00976c:	00677733          	and	a4,a4,t1
1c009770:	01c04463          	bgtz	t3,1c009778 <pos_libc_prf+0xb9c>
1c009774:	3a20106f          	j	1c00ab16 <pos_libc_prf+0x1f3a>
1c009778:	01ef5e93          	srli	t4,t5,0x1e
1c00977c:	078e                	slli	a5,a5,0x3
1c00977e:	00271e13          	slli	t3,a4,0x2
1c009782:	9f3e                	add	t5,t5,a5
1c009784:	01ceee33          	or	t3,t4,t3
1c009788:	9772                	add	a4,a4,t3
1c00978a:	00ff37b3          	sltu	a5,t5,a5
1c00978e:	97ba                	add	a5,a5,a4
1c009790:	0786                	slli	a5,a5,0x1
1c009792:	01ff5713          	srli	a4,t5,0x1f
1c009796:	8fd9                	or	a5,a5,a4
1c009798:	01c7de13          	srli	t3,a5,0x1c
1c00979c:	030e0e13          	addi	t3,t3,48
1c0097a0:	01c605a3          	sb	t3,11(a2)
1c0097a4:	ff440e13          	addi	t3,s0,-12
1c0097a8:	001f1e93          	slli	t4,t5,0x1
1c0097ac:	0067f7b3          	and	a5,a5,t1
1c0097b0:	01c04463          	bgtz	t3,1c0097b8 <pos_libc_prf+0xbdc>
1c0097b4:	3580106f          	j	1c00ab0c <pos_libc_prf+0x1f30>
1c0097b8:	003f1713          	slli	a4,t5,0x3
1c0097bc:	00279e13          	slli	t3,a5,0x2
1c0097c0:	01eedf13          	srli	t5,t4,0x1e
1c0097c4:	01cf6e33          	or	t3,t5,t3
1c0097c8:	9eba                	add	t4,t4,a4
1c0097ca:	97f2                	add	a5,a5,t3
1c0097cc:	00eeb733          	sltu	a4,t4,a4
1c0097d0:	973e                	add	a4,a4,a5
1c0097d2:	0706                	slli	a4,a4,0x1
1c0097d4:	01fed793          	srli	a5,t4,0x1f
1c0097d8:	8f5d                	or	a4,a4,a5
1c0097da:	01c75e13          	srli	t3,a4,0x1c
1c0097de:	030e0e13          	addi	t3,t3,48
1c0097e2:	01c60623          	sb	t3,12(a2)
1c0097e6:	ff340793          	addi	a5,s0,-13
1c0097ea:	001e9e13          	slli	t3,t4,0x1
1c0097ee:	00677733          	and	a4,a4,t1
1c0097f2:	00f04463          	bgtz	a5,1c0097fa <pos_libc_prf+0xc1e>
1c0097f6:	3480106f          	j	1c00ab3e <pos_libc_prf+0x1f62>
1c0097fa:	003e9793          	slli	a5,t4,0x3
1c0097fe:	01ee5f13          	srli	t5,t3,0x1e
1c009802:	00271e93          	slli	t4,a4,0x2
1c009806:	9e3e                	add	t3,t3,a5
1c009808:	01df6eb3          	or	t4,t5,t4
1c00980c:	9776                	add	a4,a4,t4
1c00980e:	00fe37b3          	sltu	a5,t3,a5
1c009812:	97ba                	add	a5,a5,a4
1c009814:	0786                	slli	a5,a5,0x1
1c009816:	01fe5713          	srli	a4,t3,0x1f
1c00981a:	8fd9                	or	a5,a5,a4
1c00981c:	01c7d713          	srli	a4,a5,0x1c
1c009820:	03070713          	addi	a4,a4,48
1c009824:	00e606a3          	sb	a4,13(a2)
1c009828:	ff240e93          	addi	t4,s0,-14
1c00982c:	001e1713          	slli	a4,t3,0x1
1c009830:	0067f7b3          	and	a5,a5,t1
1c009834:	01d04463          	bgtz	t4,1c00983c <pos_libc_prf+0xc60>
1c009838:	2fc0106f          	j	1c00ab34 <pos_libc_prf+0x1f58>
1c00983c:	003e1313          	slli	t1,t3,0x3
1c009840:	01e75e93          	srli	t4,a4,0x1e
1c009844:	00279e13          	slli	t3,a5,0x2
1c009848:	01ceee33          	or	t3,t4,t3
1c00984c:	971a                	add	a4,a4,t1
1c00984e:	00673733          	sltu	a4,a4,t1
1c009852:	97f2                	add	a5,a5,t3
1c009854:	97ba                	add	a5,a5,a4
1c009856:	0786                	slli	a5,a5,0x1
1c009858:	83f1                	srli	a5,a5,0x1c
1c00985a:	03078793          	addi	a5,a5,48
1c00985e:	00f60713          	addi	a4,a2,15
1c009862:	00f60723          	sb	a5,14(a2)
1c009866:	1445                	addi	s0,s0,-15
1c009868:	5e0584e3          	beqz	a1,1c00a650 <pos_libc_prf+0x1a74>
1c00986c:	4e81                	li	t4,0
1c00986e:	d402                	sw	zero,40(sp)
1c009870:	cc02                	sw	zero,24(sp)
1c009872:	03000593          	li	a1,48
1c009876:	fff74783          	lbu	a5,-1(a4)
1c00987a:	863a                	mv	a2,a4
1c00987c:	177d                	addi	a4,a4,-1
1c00987e:	feb78ce3          	beq	a5,a1,1c009876 <pos_libc_prf+0xc9a>
1c009882:	02e00593          	li	a1,46
1c009886:	4401                	li	s0,0
1c009888:	00b78363          	beq	a5,a1,1c00988e <pos_libc_prf+0xcb2>
1c00988c:	8732                	mv	a4,a2
1c00988e:	0df6f793          	andi	a5,a3,223
1c009892:	04500613          	li	a2,69
1c009896:	3ac78563          	beq	a5,a2,1c009c40 <pos_libc_prf+0x1064>
1c00989a:	185c                	addi	a5,sp,52
1c00989c:	40f709b3          	sub	s3,a4,a5
1c0098a0:	00070023          	sb	zero,0(a4)
1c0098a4:	99f6                	add	s3,s3,t4
1c0098a6:	47d2                	lw	a5,20(sp)
1c0098a8:	2e079563          	bnez	a5,1c009b92 <pos_libc_prf+0xfb6>
1c0098ac:	47c2                	lw	a5,16(sp)
1c0098ae:	2e079263          	bnez	a5,1c009b92 <pos_libc_prf+0xfb6>
1c0098b2:	03414783          	lbu	a5,52(sp)
1c0098b6:	02d00713          	li	a4,45
1c0098ba:	c802                	sw	zero,16(sp)
1c0098bc:	2ce78b63          	beq	a5,a4,1c009b92 <pos_libc_prf+0xfb6>
1c0098c0:	fd078793          	addi	a5,a5,-48
1c0098c4:	4725                	li	a4,9
1c0098c6:	413d8933          	sub	s2,s11,s3
1c0098ca:	f4f77263          	bgeu	a4,a5,1c00900e <pos_libc_prf+0x432>
1c0098ce:	03410c13          	addi	s8,sp,52
1c0098d2:	ca02                	sw	zero,20(sp)
1c0098d4:	d58ff06f          	j	1c008e2c <pos_libc_prf+0x250>
1c0098d8:	47c2                	lw	a5,16(sp)
1c0098da:	41340433          	sub	s0,s0,s3
1c0098de:	03410c13          	addi	s8,sp,52
1c0098e2:	97a2                	add	a5,a5,s0
1c0098e4:	0407e7b3          	p.max	a5,a5,zero
1c0098e8:	ca3e                	sw	a5,20(sp)
1c0098ea:	97ce                	add	a5,a5,s3
1c0098ec:	40fd8933          	sub	s2,s11,a5
1c0098f0:	d36ff06f          	j	1c008e26 <pos_libc_prf+0x24a>
1c0098f4:	47e2                	lw	a5,24(sp)
1c0098f6:	079d                	addi	a5,a5,7
1c0098f8:	c407b7b3          	p.bclr	a5,a5,2,0
1c0098fc:	8d3e                	mv	s10,a5
1c0098fe:	008d258b          	p.lw	a1,8(s10!)
1c009902:	43dc                	lw	a5,4(a5)
1c009904:	0155d513          	srli	a0,a1,0x15
1c009908:	00b79713          	slli	a4,a5,0xb
1c00990c:	8f49                	or	a4,a4,a0
1c00990e:	0147d613          	srli	a2,a5,0x14
1c009912:	e8b63633          	p.bclr	a2,a2,20,11
1c009916:	00b59513          	slli	a0,a1,0xb
1c00991a:	c1f73733          	p.bclr	a4,a4,0,31
1c00991e:	3807cf63          	bltz	a5,1c009cbc <pos_libc_prf+0x10e0>
1c009922:	47d2                	lw	a5,20(sp)
1c009924:	42078963          	beqz	a5,1c009d56 <pos_libc_prf+0x117a>
1c009928:	02b00793          	li	a5,43
1c00992c:	02f10a23          	sb	a5,52(sp)
1c009930:	03510313          	addi	t1,sp,53
1c009934:	7ff00793          	li	a5,2047
1c009938:	64f60ce3          	beq	a2,a5,1c00a790 <pos_libc_prf+0x1bb4>
1c00993c:	04600793          	li	a5,70
1c009940:	0df6ff93          	andi	t6,a3,223
1c009944:	00f69663          	bne	a3,a5,1c009950 <pos_libc_prf+0xd74>
1c009948:	04600f93          	li	t6,70
1c00994c:	06600693          	li	a3,102
1c009950:	41f65593          	srai	a1,a2,0x1f
1c009954:	00a66eb3          	or	t4,a2,a0
1c009958:	00e5ef33          	or	t5,a1,a4
1c00995c:	01eee7b3          	or	a5,t4,t5
1c009960:	98078be3          	beqz	a5,1c0092f6 <pos_libc_prf+0x71a>
1c009964:	6a0611e3          	bnez	a2,1c00a806 <pos_libc_prf+0x1c2a>
1c009968:	01f55793          	srli	a5,a0,0x1f
1c00996c:	00171f13          	slli	t5,a4,0x1
1c009970:	01e7ef33          	or	t5,a5,t5
1c009974:	00151e93          	slli	t4,a0,0x1
1c009978:	000f4c63          	bltz	t5,1c009990 <pos_libc_prf+0xdb4>
1c00997c:	01fed793          	srli	a5,t4,0x1f
1c009980:	001f1713          	slli	a4,t5,0x1
1c009984:	167d                	addi	a2,a2,-1
1c009986:	00e7ef33          	or	t5,a5,a4
1c00998a:	0e86                	slli	t4,t4,0x1
1c00998c:	fe0758e3          	bgez	a4,1c00997c <pos_libc_prf+0xda0>
1c009990:	c0260613          	addi	a2,a2,-1022
1c009994:	33333e37          	lui	t3,0x33333
1c009998:	800002b7          	lui	t0,0x80000
1c00999c:	4501                	li	a0,0
1c00999e:	332e0e13          	addi	t3,t3,818 # 33333332 <__l2_shared_end+0x17322f2a>
1c0099a2:	fff2c293          	not	t0,t0
1c0099a6:	53f9                	li	t2,-2
1c0099a8:	01ff1593          	slli	a1,t5,0x1f
1c0099ac:	001ed713          	srli	a4,t4,0x1
1c0099b0:	fc1eb7b3          	p.bclr	a5,t4,30,1
1c0099b4:	8f4d                	or	a4,a4,a1
1c0099b6:	00e78eb3          	add	t4,a5,a4
1c0099ba:	00feb7b3          	sltu	a5,t4,a5
1c0099be:	001f5f13          	srli	t5,t5,0x1
1c0099c2:	9f3e                	add	t5,t5,a5
1c0099c4:	87b2                	mv	a5,a2
1c0099c6:	0605                	addi	a2,a2,1
1c0099c8:	ffee60e3          	bltu	t3,t5,1c0099a8 <pos_libc_prf+0xdcc>
1c0099cc:	002e9613          	slli	a2,t4,0x2
1c0099d0:	01eed593          	srli	a1,t4,0x1e
1c0099d4:	002f1713          	slli	a4,t5,0x2
1c0099d8:	9eb2                	add	t4,t4,a2
1c0099da:	8f4d                	or	a4,a4,a1
1c0099dc:	00ceb633          	sltu	a2,t4,a2
1c0099e0:	9f3a                	add	t5,t5,a4
1c0099e2:	9f32                	add	t5,t5,a2
1c0099e4:	01fed713          	srli	a4,t4,0x1f
1c0099e8:	00278613          	addi	a2,a5,2
1c0099ec:	157d                	addi	a0,a0,-1
1c0099ee:	005f1763          	bne	t5,t0,1c0099fc <pos_libc_prf+0xe20>
1c0099f2:	00178613          	addi	a2,a5,1
1c0099f6:	0e86                	slli	t4,t4,0x1
1c0099f8:	fc174f33          	p.bset	t5,a4,30,1
1c0099fc:	fa7646e3          	blt	a2,t2,1c0099a8 <pos_libc_prf+0xdcc>
1c009a00:	80000e37          	lui	t3,0x80000
1c009a04:	4295                	li	t0,5
1c009a06:	fffe4e13          	not	t3,t3
1c009a0a:	8ec058e3          	blez	a2,1c0092fa <pos_libc_prf+0x71e>
1c009a0e:	002e8713          	addi	a4,t4,2
1c009a12:	01d73eb3          	sltu	t4,a4,t4
1c009a16:	9f76                	add	t5,t5,t4
1c009a18:	025f57b3          	divu	a5,t5,t0
1c009a1c:	00375593          	srli	a1,a4,0x3
1c009a20:	167d                	addi	a2,a2,-1
1c009a22:	0505                	addi	a0,a0,1
1c009a24:	00279813          	slli	a6,a5,0x2
1c009a28:	983e                	add	a6,a6,a5
1c009a2a:	410f0f33          	sub	t5,t5,a6
1c009a2e:	0f76                	slli	t5,t5,0x1d
1c009a30:	00bf65b3          	or	a1,t5,a1
1c009a34:	0255d5b3          	divu	a1,a1,t0
1c009a38:	00359e93          	slli	t4,a1,0x3
1c009a3c:	00559813          	slli	a6,a1,0x5
1c009a40:	9876                	add	a6,a6,t4
1c009a42:	41070733          	sub	a4,a4,a6
1c009a46:	02575733          	divu	a4,a4,t0
1c009a4a:	01d5df13          	srli	t5,a1,0x1d
1c009a4e:	97fa                	add	a5,a5,t5
1c009a50:	9eba                	add	t4,t4,a4
1c009a52:	00eeb733          	sltu	a4,t4,a4
1c009a56:	00f70f33          	add	t5,a4,a5
1c009a5a:	01fed793          	srli	a5,t4,0x1f
1c009a5e:	0f06                	slli	t5,t5,0x1
1c009a60:	01e7ef33          	or	t5,a5,t5
1c009a64:	0e86                	slli	t4,t4,0x1
1c009a66:	167d                	addi	a2,a2,-1
1c009a68:	ffee79e3          	bgeu	t3,t5,1c009a5a <pos_libc_prf+0xe7e>
1c009a6c:	bf79                	j	1c009a0a <pos_libc_prf+0xe2e>
1c009a6e:	4d62                	lw	s10,24(sp)
1c009a70:	47f2                	lw	a5,28(sp)
1c009a72:	4681                	li	a3,0
1c009a74:	004d258b          	p.lw	a1,4(s10!)
1c009a78:	02f11a23          	sh	a5,52(sp)
1c009a7c:	03610513          	addi	a0,sp,54
1c009a80:	4ea5                	li	t4,9
1c009a82:	4f3d                	li	t5,15
1c009a84:	a039                	j	1c009a92 <pos_libc_prf+0xeb6>
1c009a86:	011980ab          	p.sb	a7,1(s3!)
1c009a8a:	c69d                	beqz	a3,1c009ab8 <pos_libc_prf+0xedc>
1c009a8c:	85b2                	mv	a1,a2
1c009a8e:	86ba                	mv	a3,a4
1c009a90:	854e                	mv	a0,s3
1c009a92:	f645b7b3          	p.bclr	a5,a1,27,4
1c009a96:	01c69713          	slli	a4,a3,0x1c
1c009a9a:	0045d613          	srli	a2,a1,0x4
1c009a9e:	8e59                	or	a2,a2,a4
1c009aa0:	89aa                	mv	s3,a0
1c009aa2:	03078893          	addi	a7,a5,48
1c009aa6:	05778813          	addi	a6,a5,87
1c009aaa:	0046d713          	srli	a4,a3,0x4
1c009aae:	fcfefce3          	bgeu	t4,a5,1c009a86 <pos_libc_prf+0xeaa>
1c009ab2:	010980ab          	p.sb	a6,1(s3!)
1c009ab6:	faf9                	bnez	a3,1c009a8c <pos_libc_prf+0xeb0>
1c009ab8:	fcbf6ae3          	bltu	t5,a1,1c009a8c <pos_libc_prf+0xeb0>
1c009abc:	00098023          	sb	zero,0(s3)
1c009ac0:	03610793          	addi	a5,sp,54
1c009ac4:	0aa7fd63          	bgeu	a5,a0,1c009b7e <pos_libc_prf+0xfa2>
1c009ac8:	5782                	lw	a5,32(sp)
1c009aca:	1858                	addi	a4,sp,52
1c009acc:	00a785b3          	add	a1,a5,a0
1c009ad0:	0015d693          	srli	a3,a1,0x1
1c009ad4:	00368793          	addi	a5,a3,3
1c009ad8:	97ba                	add	a5,a5,a4
1c009ada:	40d50633          	sub	a2,a0,a3
1c009ade:	04c7b633          	p.sletu	a2,a5,a2
1c009ae2:	03610793          	addi	a5,sp,54
1c009ae6:	04f9b7b3          	p.sletu	a5,s3,a5
1c009aea:	8e5d                	or	a2,a2,a5
1c009aec:	872a                	mv	a4,a0
1c009aee:	03610793          	addi	a5,sp,54
1c009af2:	62060ee3          	beqz	a2,1c00a92e <pos_libc_prf+0x1d52>
1c009af6:	0065b593          	sltiu	a1,a1,6
1c009afa:	62059ae3          	bnez	a1,1c00a92e <pos_libc_prf+0x1d52>
1c009afe:	00168313          	addi	t1,a3,1
1c009b02:	c2033633          	p.bclr	a2,t1,1,0
1c009b06:	1671                	addi	a2,a2,-4
1c009b08:	8209                	srli	a2,a2,0x2
1c009b0a:	ffd50593          	addi	a1,a0,-3
1c009b0e:	0605                	addi	a2,a2,1
1c009b10:	00a640fb          	lp.setup	x1,a2,1c009b24 <pos_libc_prf+0xf48>
1c009b14:	4394                	lw	a3,0(a5)
1c009b16:	4198                	lw	a4,0(a1)
1c009b18:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c009b1c:	fed5ae2b          	p.sw	a3,-4(a1!)
1c009b20:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c009b24:	00e7a22b          	p.sw	a4,4(a5!)
1c009b28:	c20336b3          	p.bclr	a3,t1,1,0
1c009b2c:	03610713          	addi	a4,sp,54
1c009b30:	40d507b3          	sub	a5,a0,a3
1c009b34:	9736                	add	a4,a4,a3
1c009b36:	04668463          	beq	a3,t1,1c009b7e <pos_libc_prf+0xfa2>
1c009b3a:	00074603          	lbu	a2,0(a4)
1c009b3e:	0007c583          	lbu	a1,0(a5)
1c009b42:	86be                	mv	a3,a5
1c009b44:	fec68fab          	p.sb	a2,-1(a3!)
1c009b48:	863a                	mv	a2,a4
1c009b4a:	00b600ab          	p.sb	a1,1(a2!)
1c009b4e:	02d67863          	bgeu	a2,a3,1c009b7e <pos_libc_prf+0xfa2>
1c009b52:	00174583          	lbu	a1,1(a4)
1c009b56:	fff7c603          	lbu	a2,-1(a5)
1c009b5a:	00270693          	addi	a3,a4,2
1c009b5e:	feb78fa3          	sb	a1,-1(a5)
1c009b62:	00c700a3          	sb	a2,1(a4)
1c009b66:	ffe78613          	addi	a2,a5,-2
1c009b6a:	00c6fa63          	bgeu	a3,a2,1c009b7e <pos_libc_prf+0xfa2>
1c009b6e:	00274603          	lbu	a2,2(a4)
1c009b72:	ffe7c683          	lbu	a3,-2(a5)
1c009b76:	fec78f23          	sb	a2,-2(a5)
1c009b7a:	00d70123          	sb	a3,2(a4)
1c009b7e:	185c                	addi	a5,sp,52
1c009b80:	40f989b3          	sub	s3,s3,a5
1c009b84:	4789                	li	a5,2
1c009b86:	c83e                	sw	a5,16(sp)
1c009b88:	c78ff06f          	j	1c009000 <pos_libc_prf+0x424>
1c009b8c:	4b01                	li	s6,0
1c009b8e:	908ff06f          	j	1c008c96 <pos_libc_prf+0xba>
1c009b92:	4705                	li	a4,1
1c009b94:	03514783          	lbu	a5,53(sp)
1c009b98:	c83a                	sw	a4,16(sp)
1c009b9a:	b31d                	j	1c0098c0 <pos_libc_prf+0xce4>
1c009b9c:	01672023          	sw	s6,0(a4)
1c009ba0:	896ff06f          	j	1c008c36 <pos_libc_prf+0x5a>
1c009ba4:	4785                	li	a5,1
1c009ba6:	c83e                	sw	a5,16(sp)
1c009ba8:	c58ff06f          	j	1c009000 <pos_libc_prf+0x424>
1c009bac:	8aa2                	mv	s5,s0
1c009bae:	8bb6                	mv	s7,a3
1c009bb0:	a029                	j	1c009bba <pos_libc_prf+0xfde>
1c009bb2:	9a02                	jalr	s4
1c009bb4:	1afd                	addi	s5,s5,-1
1c009bb6:	a1f52363          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c009bba:	85a6                	mv	a1,s1
1c009bbc:	03000513          	li	a0,48
1c009bc0:	ff5049e3          	bgtz	s5,1c009bb2 <pos_libc_prf+0xfd6>
1c009bc4:	408b8c33          	sub	s8,s7,s0
1c009bc8:	8aee                	mv	s5,s11
1c009bca:	9c56                	add	s8,s8,s5
1c009bcc:	a031                	j	1c009bd8 <pos_libc_prf+0xffc>
1c009bce:	001ac50b          	p.lbu	a0,1(s5!)
1c009bd2:	9a02                	jalr	s4
1c009bd4:	9ff52463          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c009bd8:	415c07b3          	sub	a5,s8,s5
1c009bdc:	85a6                	mv	a1,s1
1c009bde:	fef048e3          	bgtz	a5,1c009bce <pos_libc_prf+0xff2>
1c009be2:	47c2                	lw	a5,16(sp)
1c009be4:	cc6a                	sw	s10,24(sp)
1c009be6:	9b3e                	add	s6,s6,a5
1c009be8:	47d2                	lw	a5,20(sp)
1c009bea:	9b3e                	add	s6,s6,a5
1c009bec:	9b4e                	add	s6,s6,s3
1c009bee:	85205463          	blez	s2,1c008c36 <pos_libc_prf+0x5a>
1c009bf2:	fff90413          	addi	s0,s2,-1
1c009bf6:	85a6                	mv	a1,s1
1c009bf8:	02000513          	li	a0,32
1c009bfc:	9a02                	jalr	s4
1c009bfe:	147d                	addi	s0,s0,-1
1c009c00:	9bf52e63          	p.beqimm	a0,-1,1c008dbc <pos_libc_prf+0x1e0>
1c009c04:	fff439e3          	p.bneimm	s0,-1,1c009bf6 <pos_libc_prf+0x101a>
1c009c08:	9b4a                	add	s6,s6,s2
1c009c0a:	cc6a                	sw	s10,24(sp)
1c009c0c:	82aff06f          	j	1c008c36 <pos_libc_prf+0x5a>
1c009c10:	57f5                	li	a5,-3
1c009c12:	0ef54963          	blt	a0,a5,1c009d04 <pos_libc_prf+0x1128>
1c009c16:	0ea44763          	blt	s0,a0,1c009d04 <pos_libc_prf+0x1128>
1c009c1a:	57b2                	lw	a5,44(sp)
1c009c1c:	40a406b3          	sub	a3,s0,a0
1c009c20:	620791e3          	bnez	a5,1c00aa42 <pos_libc_prf+0x1e66>
1c009c24:	87a2                	mv	a5,s0
1c009c26:	00d035b3          	snez	a1,a3
1c009c2a:	8436                	mv	s0,a3
1c009c2c:	58f05fe3          	blez	a5,1c00a9ca <pos_libc_prf+0x1dee>
1c009c30:	46c1                	li	a3,16
1c009c32:	04d7ceb3          	p.min	t4,a5,a3
1c009c36:	1efd                	addi	t4,t4,-1
1c009c38:	06600693          	li	a3,102
1c009c3c:	f14ff06f          	j	1c009350 <pos_libc_prf+0x774>
1c009c40:	863a                	mv	a2,a4
1c009c42:	00d6012b          	p.sb	a3,2(a2!)
1c009c46:	02b00793          	li	a5,43
1c009c4a:	00055663          	bgez	a0,1c009c56 <pos_libc_prf+0x107a>
1c009c4e:	40a00533          	neg	a0,a0
1c009c52:	02d00793          	li	a5,45
1c009c56:	00f700a3          	sb	a5,1(a4)
1c009c5a:	06300793          	li	a5,99
1c009c5e:	00a7de63          	bge	a5,a0,1c009c7a <pos_libc_prf+0x109e>
1c009c62:	06400693          	li	a3,100
1c009c66:	02d547b3          	div	a5,a0,a3
1c009c6a:	00370613          	addi	a2,a4,3
1c009c6e:	02d56533          	rem	a0,a0,a3
1c009c72:	03078793          	addi	a5,a5,48
1c009c76:	00f70123          	sb	a5,2(a4)
1c009c7a:	47a9                	li	a5,10
1c009c7c:	8732                	mv	a4,a2
1c009c7e:	02f546b3          	div	a3,a0,a5
1c009c82:	02f567b3          	rem	a5,a0,a5
1c009c86:	03068693          	addi	a3,a3,48
1c009c8a:	00d7012b          	p.sb	a3,2(a4!)
1c009c8e:	03078793          	addi	a5,a5,48
1c009c92:	00f600a3          	sb	a5,1(a2)
1c009c96:	b111                	j	1c00989a <pos_libc_prf+0xcbe>
1c009c98:	00f03733          	snez	a4,a5
1c009c9c:	40d009b3          	neg	s3,a3
1c009ca0:	40e989b3          	sub	s3,s3,a4
1c009ca4:	02d00693          	li	a3,45
1c009ca8:	03510713          	addi	a4,sp,53
1c009cac:	02d10a23          	sb	a3,52(sp)
1c009cb0:	40f007b3          	neg	a5,a5
1c009cb4:	cc3a                	sw	a4,24(sp)
1c009cb6:	893a                	mv	s2,a4
1c009cb8:	a44ff06f          	j	1c008efc <pos_libc_prf+0x320>
1c009cbc:	02d00793          	li	a5,45
1c009cc0:	02f10a23          	sb	a5,52(sp)
1c009cc4:	03510313          	addi	t1,sp,53
1c009cc8:	b1b5                	j	1c009934 <pos_libc_prf+0xd58>
1c009cca:	000ac503          	lbu	a0,0(s5)
1c009cce:	47a5                	li	a5,9
1c009cd0:	fd050713          	addi	a4,a0,-48
1c009cd4:	48e7ece3          	bltu	a5,a4,1c00a96c <pos_libc_prf+0x1d90>
1c009cd8:	001a8d93          	addi	s11,s5,1
1c009cdc:	013a8bb3          	add	s7,s5,s3
1c009ce0:	a801                	j	1c009cf0 <pos_libc_prf+0x1114>
1c009ce2:	001dc50b          	p.lbu	a0,1(s11!)
1c009ce6:	4725                	li	a4,9
1c009ce8:	fd050793          	addi	a5,a0,-48
1c009cec:	48f760e3          	bltu	a4,a5,1c00a96c <pos_libc_prf+0x1d90>
1c009cf0:	85a6                	mv	a1,s1
1c009cf2:	41bb8c33          	sub	s8,s7,s11
1c009cf6:	8aee                	mv	s5,s11
1c009cf8:	9a02                	jalr	s4
1c009cfa:	fff534e3          	p.bneimm	a0,-1,1c009ce2 <pos_libc_prf+0x1106>
1c009cfe:	5b7d                	li	s6,-1
1c009d00:	f97fe06f          	j	1c008c96 <pos_libc_prf+0xba>
1c009d04:	16f9                	addi	a3,a3,-2
1c009d06:	0ff6f693          	andi	a3,a3,255
1c009d0a:	180400e3          	beqz	s0,1c00a68a <pos_libc_prf+0x1aae>
1c009d0e:	47c1                	li	a5,16
1c009d10:	04f44eb3          	p.min	t4,s0,a5
1c009d14:	57b2                	lw	a5,44(sp)
1c009d16:	147d                	addi	s0,s0,-1
1c009d18:	1efd                	addi	t4,t4,-1
1c009d1a:	160799e3          	bnez	a5,1c00a68c <pos_libc_prf+0x1ab0>
1c009d1e:	008035b3          	snez	a1,s0
1c009d22:	e2eff06f          	j	1c009350 <pos_libc_prf+0x774>
1c009d26:	02e00713          	li	a4,46
1c009d2a:	00e300a3          	sb	a4,1(t1) # 10000001 <__l1_heap_size+0xffc0021>
1c009d2e:	00230613          	addi	a2,t1,2
1c009d32:	f6041563          	bnez	s0,1c00949c <pos_libc_prf+0x8c0>
1c009d36:	8732                	mv	a4,a2
1c009d38:	be05                	j	1c009868 <pos_libc_prf+0xc8c>
1c009d3a:	4742                	lw	a4,16(sp)
1c009d3c:	89b6                	mv	s3,a3
1c009d3e:	44070ee3          	beqz	a4,1c00a99a <pos_libc_prf+0x1dbe>
1c009d42:	02000713          	li	a4,32
1c009d46:	02e10a23          	sb	a4,52(sp)
1c009d4a:	03510713          	addi	a4,sp,53
1c009d4e:	cc3a                	sw	a4,24(sp)
1c009d50:	893a                	mv	s2,a4
1c009d52:	9aaff06f          	j	1c008efc <pos_libc_prf+0x320>
1c009d56:	47c2                	lw	a5,16(sp)
1c009d58:	03410313          	addi	t1,sp,52
1c009d5c:	bc078ce3          	beqz	a5,1c009934 <pos_libc_prf+0xd58>
1c009d60:	02000793          	li	a5,32
1c009d64:	02f10a23          	sb	a5,52(sp)
1c009d68:	03510313          	addi	t1,sp,53
1c009d6c:	b6e1                	j	1c009934 <pos_libc_prf+0xd58>
1c009d6e:	46a055e3          	blez	a0,1c00a9d8 <pos_libc_prf+0x1dfc>
1c009d72:	00279e13          	slli	t3,a5,0x2
1c009d76:	01e7df93          	srli	t6,a5,0x1e
1c009d7a:	00229f13          	slli	t5,t0,0x2
1c009d7e:	00fe0eb3          	add	t4,t3,a5
1c009d82:	01efef33          	or	t5,t6,t5
1c009d86:	9f16                	add	t5,t5,t0
1c009d88:	01ceb2b3          	sltu	t0,t4,t3
1c009d8c:	92fa                	add	t0,t0,t5
1c009d8e:	01fed793          	srli	a5,t4,0x1f
1c009d92:	0286                	slli	t0,t0,0x1
1c009d94:	0057e2b3          	or	t0,a5,t0
1c009d98:	01c2d793          	srli	a5,t0,0x1c
1c009d9c:	03078793          	addi	a5,a5,48
1c009da0:	10000e37          	lui	t3,0x10000
1c009da4:	1e7d                	addi	t3,t3,-1
1c009da6:	00f30023          	sb	a5,0(t1)
1c009daa:	01c2f2b3          	and	t0,t0,t3
1c009dae:	001e9793          	slli	a5,t4,0x1
1c009db2:	621529e3          	p.beqimm	a0,1,1c00abe4 <pos_libc_prf+0x2008>
1c009db6:	01e7df93          	srli	t6,a5,0x1e
1c009dba:	00229f13          	slli	t5,t0,0x2
1c009dbe:	003e9713          	slli	a4,t4,0x3
1c009dc2:	00f70eb3          	add	t4,a4,a5
1c009dc6:	01efef33          	or	t5,t6,t5
1c009dca:	9f16                	add	t5,t5,t0
1c009dcc:	00eeb733          	sltu	a4,t4,a4
1c009dd0:	977a                	add	a4,a4,t5
1c009dd2:	01fed793          	srli	a5,t4,0x1f
1c009dd6:	0706                	slli	a4,a4,0x1
1c009dd8:	8f5d                	or	a4,a4,a5
1c009dda:	01c75793          	srli	a5,a4,0x1c
1c009dde:	03078793          	addi	a5,a5,48
1c009de2:	00f300a3          	sb	a5,1(t1)
1c009de6:	01c772b3          	and	t0,a4,t3
1c009dea:	001e9793          	slli	a5,t4,0x1
1c009dee:	5e2525e3          	p.beqimm	a0,2,1c00abd8 <pos_libc_prf+0x1ffc>
1c009df2:	01e7df93          	srli	t6,a5,0x1e
1c009df6:	00229f13          	slli	t5,t0,0x2
1c009dfa:	0e8e                	slli	t4,t4,0x3
1c009dfc:	00fe8733          	add	a4,t4,a5
1c009e00:	01efef33          	or	t5,t6,t5
1c009e04:	9f16                	add	t5,t5,t0
1c009e06:	01d732b3          	sltu	t0,a4,t4
1c009e0a:	92fa                	add	t0,t0,t5
1c009e0c:	01f75793          	srli	a5,a4,0x1f
1c009e10:	0286                	slli	t0,t0,0x1
1c009e12:	0057e2b3          	or	t0,a5,t0
1c009e16:	01c2d793          	srli	a5,t0,0x1c
1c009e1a:	03078793          	addi	a5,a5,48
1c009e1e:	ffd50813          	addi	a6,a0,-3
1c009e22:	00f30123          	sb	a5,2(t1)
1c009e26:	cc42                	sw	a6,24(sp)
1c009e28:	00171793          	slli	a5,a4,0x1
1c009e2c:	01c2f2b3          	and	t0,t0,t3
1c009e30:	59005fe3          	blez	a6,1c00abce <pos_libc_prf+0x1ff2>
1c009e34:	01e7df93          	srli	t6,a5,0x1e
1c009e38:	00229f13          	slli	t5,t0,0x2
1c009e3c:	070e                	slli	a4,a4,0x3
1c009e3e:	00f70eb3          	add	t4,a4,a5
1c009e42:	01efef33          	or	t5,t6,t5
1c009e46:	9f16                	add	t5,t5,t0
1c009e48:	00eeb733          	sltu	a4,t4,a4
1c009e4c:	977a                	add	a4,a4,t5
1c009e4e:	01fed793          	srli	a5,t4,0x1f
1c009e52:	0706                	slli	a4,a4,0x1
1c009e54:	8f5d                	or	a4,a4,a5
1c009e56:	01c75793          	srli	a5,a4,0x1c
1c009e5a:	03078793          	addi	a5,a5,48
1c009e5e:	ffc50813          	addi	a6,a0,-4
1c009e62:	00f301a3          	sb	a5,3(t1)
1c009e66:	cc42                	sw	a6,24(sp)
1c009e68:	001e9793          	slli	a5,t4,0x1
1c009e6c:	01c772b3          	and	t0,a4,t3
1c009e70:	55005ae3          	blez	a6,1c00abc4 <pos_libc_prf+0x1fe8>
1c009e74:	01e7df93          	srli	t6,a5,0x1e
1c009e78:	00229f13          	slli	t5,t0,0x2
1c009e7c:	0e8e                	slli	t4,t4,0x3
1c009e7e:	00fe8733          	add	a4,t4,a5
1c009e82:	01efef33          	or	t5,t6,t5
1c009e86:	9f16                	add	t5,t5,t0
1c009e88:	01d732b3          	sltu	t0,a4,t4
1c009e8c:	92fa                	add	t0,t0,t5
1c009e8e:	01f75793          	srli	a5,a4,0x1f
1c009e92:	0286                	slli	t0,t0,0x1
1c009e94:	0057e2b3          	or	t0,a5,t0
1c009e98:	01c2d793          	srli	a5,t0,0x1c
1c009e9c:	03078793          	addi	a5,a5,48
1c009ea0:	ffb50813          	addi	a6,a0,-5
1c009ea4:	00f30223          	sb	a5,4(t1)
1c009ea8:	cc42                	sw	a6,24(sp)
1c009eaa:	00171793          	slli	a5,a4,0x1
1c009eae:	01c2f2b3          	and	t0,t0,t3
1c009eb2:	510054e3          	blez	a6,1c00abba <pos_libc_prf+0x1fde>
1c009eb6:	01e7df93          	srli	t6,a5,0x1e
1c009eba:	00229f13          	slli	t5,t0,0x2
1c009ebe:	070e                	slli	a4,a4,0x3
1c009ec0:	00f70eb3          	add	t4,a4,a5
1c009ec4:	01efef33          	or	t5,t6,t5
1c009ec8:	9f16                	add	t5,t5,t0
1c009eca:	00eeb733          	sltu	a4,t4,a4
1c009ece:	977a                	add	a4,a4,t5
1c009ed0:	01fed793          	srli	a5,t4,0x1f
1c009ed4:	0706                	slli	a4,a4,0x1
1c009ed6:	8f5d                	or	a4,a4,a5
1c009ed8:	01c75793          	srli	a5,a4,0x1c
1c009edc:	03078793          	addi	a5,a5,48
1c009ee0:	ffa50813          	addi	a6,a0,-6
1c009ee4:	00f302a3          	sb	a5,5(t1)
1c009ee8:	cc42                	sw	a6,24(sp)
1c009eea:	001e9793          	slli	a5,t4,0x1
1c009eee:	01c772b3          	and	t0,a4,t3
1c009ef2:	4b005fe3          	blez	a6,1c00abb0 <pos_libc_prf+0x1fd4>
1c009ef6:	01e7df93          	srli	t6,a5,0x1e
1c009efa:	00229f13          	slli	t5,t0,0x2
1c009efe:	0e8e                	slli	t4,t4,0x3
1c009f00:	00fe8733          	add	a4,t4,a5
1c009f04:	01efef33          	or	t5,t6,t5
1c009f08:	9f16                	add	t5,t5,t0
1c009f0a:	01d732b3          	sltu	t0,a4,t4
1c009f0e:	92fa                	add	t0,t0,t5
1c009f10:	01f75793          	srli	a5,a4,0x1f
1c009f14:	0286                	slli	t0,t0,0x1
1c009f16:	0057e2b3          	or	t0,a5,t0
1c009f1a:	01c2d793          	srli	a5,t0,0x1c
1c009f1e:	03078793          	addi	a5,a5,48
1c009f22:	ff950813          	addi	a6,a0,-7
1c009f26:	00f30323          	sb	a5,6(t1)
1c009f2a:	cc42                	sw	a6,24(sp)
1c009f2c:	00171793          	slli	a5,a4,0x1
1c009f30:	01c2f2b3          	and	t0,t0,t3
1c009f34:	470059e3          	blez	a6,1c00aba6 <pos_libc_prf+0x1fca>
1c009f38:	01e7df13          	srli	t5,a5,0x1e
1c009f3c:	00229e93          	slli	t4,t0,0x2
1c009f40:	070e                	slli	a4,a4,0x3
1c009f42:	97ba                	add	a5,a5,a4
1c009f44:	01df6eb3          	or	t4,t5,t4
1c009f48:	9e96                	add	t4,t4,t0
1c009f4a:	00e7b733          	sltu	a4,a5,a4
1c009f4e:	9776                	add	a4,a4,t4
1c009f50:	0706                	slli	a4,a4,0x1
1c009f52:	01f7de93          	srli	t4,a5,0x1f
1c009f56:	00eee733          	or	a4,t4,a4
1c009f5a:	01c75e93          	srli	t4,a4,0x1c
1c009f5e:	030e8e93          	addi	t4,t4,48
1c009f62:	ff850813          	addi	a6,a0,-8
1c009f66:	0786                	slli	a5,a5,0x1
1c009f68:	01d303a3          	sb	t4,7(t1)
1c009f6c:	cc42                	sw	a6,24(sp)
1c009f6e:	8ebe                	mv	t4,a5
1c009f70:	01c772b3          	and	t0,a4,t3
1c009f74:	430054e3          	blez	a6,1c00ab9c <pos_libc_prf+0x1fc0>
1c009f78:	078a                	slli	a5,a5,0x2
1c009f7a:	00229e13          	slli	t3,t0,0x2
1c009f7e:	01eedf13          	srli	t5,t4,0x1e
1c009f82:	01d78733          	add	a4,a5,t4
1c009f86:	01cf6e33          	or	t3,t5,t3
1c009f8a:	9e16                	add	t3,t3,t0
1c009f8c:	00f732b3          	sltu	t0,a4,a5
1c009f90:	92f2                	add	t0,t0,t3
1c009f92:	01f75793          	srli	a5,a4,0x1f
1c009f96:	0286                	slli	t0,t0,0x1
1c009f98:	0057e2b3          	or	t0,a5,t0
1c009f9c:	01c2d793          	srli	a5,t0,0x1c
1c009fa0:	03078793          	addi	a5,a5,48
1c009fa4:	10000e37          	lui	t3,0x10000
1c009fa8:	ff750813          	addi	a6,a0,-9
1c009fac:	1e7d                	addi	t3,t3,-1
1c009fae:	00f30423          	sb	a5,8(t1)
1c009fb2:	cc42                	sw	a6,24(sp)
1c009fb4:	00171793          	slli	a5,a4,0x1
1c009fb8:	01c2f2b3          	and	t0,t0,t3
1c009fbc:	3b0059e3          	blez	a6,1c00ab6e <pos_libc_prf+0x1f92>
1c009fc0:	01e7df93          	srli	t6,a5,0x1e
1c009fc4:	00229f13          	slli	t5,t0,0x2
1c009fc8:	070e                	slli	a4,a4,0x3
1c009fca:	00f70eb3          	add	t4,a4,a5
1c009fce:	01efef33          	or	t5,t6,t5
1c009fd2:	9f16                	add	t5,t5,t0
1c009fd4:	00eeb733          	sltu	a4,t4,a4
1c009fd8:	977a                	add	a4,a4,t5
1c009fda:	01fed793          	srli	a5,t4,0x1f
1c009fde:	0706                	slli	a4,a4,0x1
1c009fe0:	8f5d                	or	a4,a4,a5
1c009fe2:	01c75793          	srli	a5,a4,0x1c
1c009fe6:	03078793          	addi	a5,a5,48
1c009fea:	ff650813          	addi	a6,a0,-10
1c009fee:	00f304a3          	sb	a5,9(t1)
1c009ff2:	cc42                	sw	a6,24(sp)
1c009ff4:	001e9793          	slli	a5,t4,0x1
1c009ff8:	01c772b3          	and	t0,a4,t3
1c009ffc:	370054e3          	blez	a6,1c00ab64 <pos_libc_prf+0x1f88>
1c00a000:	01e7df93          	srli	t6,a5,0x1e
1c00a004:	00229f13          	slli	t5,t0,0x2
1c00a008:	0e8e                	slli	t4,t4,0x3
1c00a00a:	00fe8733          	add	a4,t4,a5
1c00a00e:	01efef33          	or	t5,t6,t5
1c00a012:	9f16                	add	t5,t5,t0
1c00a014:	01d732b3          	sltu	t0,a4,t4
1c00a018:	92fa                	add	t0,t0,t5
1c00a01a:	01f75793          	srli	a5,a4,0x1f
1c00a01e:	0286                	slli	t0,t0,0x1
1c00a020:	0057e2b3          	or	t0,a5,t0
1c00a024:	01c2d793          	srli	a5,t0,0x1c
1c00a028:	03078793          	addi	a5,a5,48
1c00a02c:	ff550813          	addi	a6,a0,-11
1c00a030:	00f30523          	sb	a5,10(t1)
1c00a034:	cc42                	sw	a6,24(sp)
1c00a036:	00171793          	slli	a5,a4,0x1
1c00a03a:	01c2f2b3          	and	t0,t0,t3
1c00a03e:	31005ee3          	blez	a6,1c00ab5a <pos_libc_prf+0x1f7e>
1c00a042:	01e7df93          	srli	t6,a5,0x1e
1c00a046:	00229f13          	slli	t5,t0,0x2
1c00a04a:	070e                	slli	a4,a4,0x3
1c00a04c:	00f70eb3          	add	t4,a4,a5
1c00a050:	01efef33          	or	t5,t6,t5
1c00a054:	9f16                	add	t5,t5,t0
1c00a056:	00eeb733          	sltu	a4,t4,a4
1c00a05a:	977a                	add	a4,a4,t5
1c00a05c:	01fed793          	srli	a5,t4,0x1f
1c00a060:	0706                	slli	a4,a4,0x1
1c00a062:	8f5d                	or	a4,a4,a5
1c00a064:	01c75793          	srli	a5,a4,0x1c
1c00a068:	03078793          	addi	a5,a5,48
1c00a06c:	ff450813          	addi	a6,a0,-12
1c00a070:	00f305a3          	sb	a5,11(t1)
1c00a074:	cc42                	sw	a6,24(sp)
1c00a076:	01c772b3          	and	t0,a4,t3
1c00a07a:	001e9793          	slli	a5,t4,0x1
1c00a07e:	00c30713          	addi	a4,t1,12
1c00a082:	11005563          	blez	a6,1c00a18c <pos_libc_prf+0x15b0>
1c00a086:	003e9613          	slli	a2,t4,0x3
1c00a08a:	01e7df93          	srli	t6,a5,0x1e
1c00a08e:	00229f13          	slli	t5,t0,0x2
1c00a092:	00f60733          	add	a4,a2,a5
1c00a096:	01efef33          	or	t5,t6,t5
1c00a09a:	9f16                	add	t5,t5,t0
1c00a09c:	00c732b3          	sltu	t0,a4,a2
1c00a0a0:	92fa                	add	t0,t0,t5
1c00a0a2:	01f75793          	srli	a5,a4,0x1f
1c00a0a6:	0286                	slli	t0,t0,0x1
1c00a0a8:	0057e2b3          	or	t0,a5,t0
1c00a0ac:	01c2d793          	srli	a5,t0,0x1c
1c00a0b0:	03078793          	addi	a5,a5,48
1c00a0b4:	ff350613          	addi	a2,a0,-13
1c00a0b8:	00f30623          	sb	a5,12(t1)
1c00a0bc:	cc32                	sw	a2,24(sp)
1c00a0be:	00171793          	slli	a5,a4,0x1
1c00a0c2:	01c2f2b3          	and	t0,t0,t3
1c00a0c6:	2cc056e3          	blez	a2,1c00ab92 <pos_libc_prf+0x1fb6>
1c00a0ca:	01e7df13          	srli	t5,a5,0x1e
1c00a0ce:	00229e93          	slli	t4,t0,0x2
1c00a0d2:	070e                	slli	a4,a4,0x3
1c00a0d4:	00f70633          	add	a2,a4,a5
1c00a0d8:	01df6eb3          	or	t4,t5,t4
1c00a0dc:	9e96                	add	t4,t4,t0
1c00a0de:	00e63733          	sltu	a4,a2,a4
1c00a0e2:	9776                	add	a4,a4,t4
1c00a0e4:	01f65793          	srli	a5,a2,0x1f
1c00a0e8:	0706                	slli	a4,a4,0x1
1c00a0ea:	8f5d                	or	a4,a4,a5
1c00a0ec:	01c75793          	srli	a5,a4,0x1c
1c00a0f0:	03078793          	addi	a5,a5,48
1c00a0f4:	ff250813          	addi	a6,a0,-14
1c00a0f8:	00f306a3          	sb	a5,13(t1)
1c00a0fc:	cc42                	sw	a6,24(sp)
1c00a0fe:	00161793          	slli	a5,a2,0x1
1c00a102:	01c772b3          	and	t0,a4,t3
1c00a106:	290051e3          	blez	a6,1c00ab88 <pos_libc_prf+0x1fac>
1c00a10a:	060e                	slli	a2,a2,0x3
1c00a10c:	01e7df13          	srli	t5,a5,0x1e
1c00a110:	00229e93          	slli	t4,t0,0x2
1c00a114:	00f60733          	add	a4,a2,a5
1c00a118:	01df6eb3          	or	t4,t5,t4
1c00a11c:	9e96                	add	t4,t4,t0
1c00a11e:	00c732b3          	sltu	t0,a4,a2
1c00a122:	92f6                	add	t0,t0,t4
1c00a124:	01f75793          	srli	a5,a4,0x1f
1c00a128:	0286                	slli	t0,t0,0x1
1c00a12a:	0057e2b3          	or	t0,a5,t0
1c00a12e:	01c2d793          	srli	a5,t0,0x1c
1c00a132:	03078793          	addi	a5,a5,48
1c00a136:	ff150613          	addi	a2,a0,-15
1c00a13a:	00f30723          	sb	a5,14(t1)
1c00a13e:	cc32                	sw	a2,24(sp)
1c00a140:	00171793          	slli	a5,a4,0x1
1c00a144:	01c2f2b3          	and	t0,t0,t3
1c00a148:	20c054e3          	blez	a2,1c00ab50 <pos_libc_prf+0x1f74>
1c00a14c:	070e                	slli	a4,a4,0x3
1c00a14e:	01e7de93          	srli	t4,a5,0x1e
1c00a152:	00229613          	slli	a2,t0,0x2
1c00a156:	97ba                	add	a5,a5,a4
1c00a158:	00cee633          	or	a2,t4,a2
1c00a15c:	9616                	add	a2,a2,t0
1c00a15e:	00e7b2b3          	sltu	t0,a5,a4
1c00a162:	92b2                	add	t0,t0,a2
1c00a164:	01f7d713          	srli	a4,a5,0x1f
1c00a168:	0286                	slli	t0,t0,0x1
1c00a16a:	005762b3          	or	t0,a4,t0
1c00a16e:	01c2d613          	srli	a2,t0,0x1c
1c00a172:	03060613          	addi	a2,a2,48
1c00a176:	00c307a3          	sb	a2,15(t1)
1c00a17a:	ff050613          	addi	a2,a0,-16
1c00a17e:	cc32                	sw	a2,24(sp)
1c00a180:	01030713          	addi	a4,t1,16
1c00a184:	0786                	slli	a5,a5,0x1
1c00a186:	01c2f2b3          	and	t0,t0,t3
1c00a18a:	4601                	li	a2,0
1c00a18c:	5532                	lw	a0,44(sp)
1c00a18e:	080513e3          	bnez	a0,1c00aa14 <pos_libc_prf+0x1e38>
1c00a192:	10040ee3          	beqz	s0,1c00aaae <pos_libc_prf+0x1ed2>
1c00a196:	833a                	mv	t1,a4
1c00a198:	02e00713          	li	a4,46
1c00a19c:	00e300ab          	p.sb	a4,1(t1!)
1c00a1a0:	4ee2                	lw	t4,24(sp)
1c00a1a2:	8e22                	mv	t3,s0
1c00a1a4:	4501                	li	a0,0
1c00a1a6:	d402                	sw	zero,40(sp)
1c00a1a8:	060601e3          	beqz	a2,1c00aa0a <pos_libc_prf+0x1e2e>
1c00a1ac:	00279713          	slli	a4,a5,0x2
1c00a1b0:	01e7df93          	srli	t6,a5,0x1e
1c00a1b4:	00229f13          	slli	t5,t0,0x2
1c00a1b8:	97ba                	add	a5,a5,a4
1c00a1ba:	01efef33          	or	t5,t6,t5
1c00a1be:	00e7bfb3          	sltu	t6,a5,a4
1c00a1c2:	005f0733          	add	a4,t5,t0
1c00a1c6:	9fba                	add	t6,t6,a4
1c00a1c8:	0f86                	slli	t6,t6,0x1
1c00a1ca:	01f7d713          	srli	a4,a5,0x1f
1c00a1ce:	01f76fb3          	or	t6,a4,t6
1c00a1d2:	01cfd293          	srli	t0,t6,0x1c
1c00a1d6:	03028293          	addi	t0,t0,48 # 80000030 <__l2_shared_end+0x63fefc28>
1c00a1da:	871a                	mv	a4,t1
1c00a1dc:	10000f37          	lui	t5,0x10000
1c00a1e0:	1f7d                	addi	t5,t5,-1
1c00a1e2:	005700ab          	p.sb	t0,1(a4!)
1c00a1e6:	fff60393          	addi	t2,a2,-1
1c00a1ea:	00179293          	slli	t0,a5,0x1
1c00a1ee:	01efffb3          	and	t6,t6,t5
1c00a1f2:	fffe0413          	addi	s0,t3,-1 # fffffff <__l1_heap_size+0xffc001f>
1c00a1f6:	441e2863          	p.beqimm	t3,1,1c00a646 <pos_libc_prf+0x1a6a>
1c00a1fa:	44038663          	beqz	t2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a1fe:	01e2d393          	srli	t2,t0,0x1e
1c00a202:	078e                	slli	a5,a5,0x3
1c00a204:	002f9713          	slli	a4,t6,0x2
1c00a208:	92be                	add	t0,t0,a5
1c00a20a:	00e3e733          	or	a4,t2,a4
1c00a20e:	9fba                	add	t6,t6,a4
1c00a210:	00f2b7b3          	sltu	a5,t0,a5
1c00a214:	97fe                	add	a5,a5,t6
1c00a216:	01f2d713          	srli	a4,t0,0x1f
1c00a21a:	0786                	slli	a5,a5,0x1
1c00a21c:	8fd9                	or	a5,a5,a4
1c00a21e:	01c7d713          	srli	a4,a5,0x1c
1c00a222:	03070713          	addi	a4,a4,48
1c00a226:	00e300a3          	sb	a4,1(t1)
1c00a22a:	ffe60913          	addi	s2,a2,-2
1c00a22e:	00230713          	addi	a4,t1,2
1c00a232:	00129f93          	slli	t6,t0,0x1
1c00a236:	01e7f3b3          	and	t2,a5,t5
1c00a23a:	ffee0413          	addi	s0,t3,-2
1c00a23e:	402e2463          	p.beqimm	t3,2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a242:	40090263          	beqz	s2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a246:	00329713          	slli	a4,t0,0x3
1c00a24a:	01efd793          	srli	a5,t6,0x1e
1c00a24e:	00239293          	slli	t0,t2,0x2
1c00a252:	9fba                	add	t6,t6,a4
1c00a254:	0057e2b3          	or	t0,a5,t0
1c00a258:	929e                	add	t0,t0,t2
1c00a25a:	00efb7b3          	sltu	a5,t6,a4
1c00a25e:	9796                	add	a5,a5,t0
1c00a260:	01ffd713          	srli	a4,t6,0x1f
1c00a264:	0786                	slli	a5,a5,0x1
1c00a266:	8fd9                	or	a5,a5,a4
1c00a268:	01c7d713          	srli	a4,a5,0x1c
1c00a26c:	03070713          	addi	a4,a4,48
1c00a270:	00e30123          	sb	a4,2(t1)
1c00a274:	ffd60913          	addi	s2,a2,-3
1c00a278:	00330713          	addi	a4,t1,3
1c00a27c:	001f9293          	slli	t0,t6,0x1
1c00a280:	01e7f3b3          	and	t2,a5,t5
1c00a284:	ffde0413          	addi	s0,t3,-3
1c00a288:	3a3e2f63          	p.beqimm	t3,3,1c00a646 <pos_libc_prf+0x1a6a>
1c00a28c:	3a090d63          	beqz	s2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a290:	0f8e                	slli	t6,t6,0x3
1c00a292:	01e2d793          	srli	a5,t0,0x1e
1c00a296:	00239713          	slli	a4,t2,0x2
1c00a29a:	92fe                	add	t0,t0,t6
1c00a29c:	8f5d                	or	a4,a4,a5
1c00a29e:	971e                	add	a4,a4,t2
1c00a2a0:	01f2b7b3          	sltu	a5,t0,t6
1c00a2a4:	97ba                	add	a5,a5,a4
1c00a2a6:	0786                	slli	a5,a5,0x1
1c00a2a8:	01f2d713          	srli	a4,t0,0x1f
1c00a2ac:	8fd9                	or	a5,a5,a4
1c00a2ae:	01c7d713          	srli	a4,a5,0x1c
1c00a2b2:	03070713          	addi	a4,a4,48
1c00a2b6:	00e301a3          	sb	a4,3(t1)
1c00a2ba:	ffc60913          	addi	s2,a2,-4
1c00a2be:	00430713          	addi	a4,t1,4
1c00a2c2:	00129393          	slli	t2,t0,0x1
1c00a2c6:	01e7ffb3          	and	t6,a5,t5
1c00a2ca:	ffce0413          	addi	s0,t3,-4
1c00a2ce:	364e2c63          	p.beqimm	t3,4,1c00a646 <pos_libc_prf+0x1a6a>
1c00a2d2:	36090a63          	beqz	s2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a2d6:	00329713          	slli	a4,t0,0x3
1c00a2da:	01e3d793          	srli	a5,t2,0x1e
1c00a2de:	002f9413          	slli	s0,t6,0x2
1c00a2e2:	007702b3          	add	t0,a4,t2
1c00a2e6:	8c5d                	or	s0,s0,a5
1c00a2e8:	9fa2                	add	t6,t6,s0
1c00a2ea:	00e2b7b3          	sltu	a5,t0,a4
1c00a2ee:	97fe                	add	a5,a5,t6
1c00a2f0:	01f2d713          	srli	a4,t0,0x1f
1c00a2f4:	0786                	slli	a5,a5,0x1
1c00a2f6:	8fd9                	or	a5,a5,a4
1c00a2f8:	01c7d713          	srli	a4,a5,0x1c
1c00a2fc:	03070713          	addi	a4,a4,48
1c00a300:	00e30223          	sb	a4,4(t1)
1c00a304:	ffb60393          	addi	t2,a2,-5
1c00a308:	00530713          	addi	a4,t1,5
1c00a30c:	00129f93          	slli	t6,t0,0x1
1c00a310:	01e7f7b3          	and	a5,a5,t5
1c00a314:	ffbe0413          	addi	s0,t3,-5
1c00a318:	325e2763          	p.beqimm	t3,5,1c00a646 <pos_libc_prf+0x1a6a>
1c00a31c:	32038563          	beqz	t2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a320:	01efd713          	srli	a4,t6,0x1e
1c00a324:	002f9f13          	slli	t5,t6,0x2
1c00a328:	00279393          	slli	t2,a5,0x2
1c00a32c:	9ffa                	add	t6,t6,t5
1c00a32e:	007763b3          	or	t2,a4,t2
1c00a332:	01efbf33          	sltu	t5,t6,t5
1c00a336:	979e                	add	a5,a5,t2
1c00a338:	97fa                	add	a5,a5,t5
1c00a33a:	01ffd713          	srli	a4,t6,0x1f
1c00a33e:	0786                	slli	a5,a5,0x1
1c00a340:	8fd9                	or	a5,a5,a4
1c00a342:	01c7d713          	srli	a4,a5,0x1c
1c00a346:	03070713          	addi	a4,a4,48
1c00a34a:	10000f37          	lui	t5,0x10000
1c00a34e:	1f7d                	addi	t5,t5,-1
1c00a350:	00e302a3          	sb	a4,5(t1)
1c00a354:	ffa60913          	addi	s2,a2,-6
1c00a358:	00630713          	addi	a4,t1,6
1c00a35c:	001f9393          	slli	t2,t6,0x1
1c00a360:	01e7f2b3          	and	t0,a5,t5
1c00a364:	ffae0413          	addi	s0,t3,-6
1c00a368:	2c6e2f63          	p.beqimm	t3,6,1c00a646 <pos_libc_prf+0x1a6a>
1c00a36c:	2c090d63          	beqz	s2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a370:	003f9713          	slli	a4,t6,0x3
1c00a374:	01e3d413          	srli	s0,t2,0x1e
1c00a378:	00229f93          	slli	t6,t0,0x2
1c00a37c:	007707b3          	add	a5,a4,t2
1c00a380:	01f46fb3          	or	t6,s0,t6
1c00a384:	9f96                	add	t6,t6,t0
1c00a386:	00e7b733          	sltu	a4,a5,a4
1c00a38a:	977e                	add	a4,a4,t6
1c00a38c:	01f7d293          	srli	t0,a5,0x1f
1c00a390:	00171f93          	slli	t6,a4,0x1
1c00a394:	01f2efb3          	or	t6,t0,t6
1c00a398:	01cfd713          	srli	a4,t6,0x1c
1c00a39c:	03070713          	addi	a4,a4,48
1c00a3a0:	00e30323          	sb	a4,6(t1)
1c00a3a4:	ff960293          	addi	t0,a2,-7
1c00a3a8:	00730713          	addi	a4,t1,7
1c00a3ac:	00179393          	slli	t2,a5,0x1
1c00a3b0:	01efffb3          	and	t6,t6,t5
1c00a3b4:	ff9e0413          	addi	s0,t3,-7
1c00a3b8:	287e2763          	p.beqimm	t3,7,1c00a646 <pos_libc_prf+0x1a6a>
1c00a3bc:	28028563          	beqz	t0,1c00a646 <pos_libc_prf+0x1a6a>
1c00a3c0:	01e3d293          	srli	t0,t2,0x1e
1c00a3c4:	078e                	slli	a5,a5,0x3
1c00a3c6:	002f9713          	slli	a4,t6,0x2
1c00a3ca:	93be                	add	t2,t2,a5
1c00a3cc:	00e2e733          	or	a4,t0,a4
1c00a3d0:	9fba                	add	t6,t6,a4
1c00a3d2:	00f3b7b3          	sltu	a5,t2,a5
1c00a3d6:	97fe                	add	a5,a5,t6
1c00a3d8:	01f3d713          	srli	a4,t2,0x1f
1c00a3dc:	0786                	slli	a5,a5,0x1
1c00a3de:	8fd9                	or	a5,a5,a4
1c00a3e0:	01c7d713          	srli	a4,a5,0x1c
1c00a3e4:	03070713          	addi	a4,a4,48
1c00a3e8:	00e303a3          	sb	a4,7(t1)
1c00a3ec:	ff860f93          	addi	t6,a2,-8
1c00a3f0:	00830713          	addi	a4,t1,8
1c00a3f4:	00139293          	slli	t0,t2,0x1
1c00a3f8:	01e7f7b3          	and	a5,a5,t5
1c00a3fc:	ff8e0413          	addi	s0,t3,-8
1c00a400:	248e2363          	p.beqimm	t3,8,1c00a646 <pos_libc_prf+0x1a6a>
1c00a404:	240f8163          	beqz	t6,1c00a646 <pos_libc_prf+0x1a6a>
1c00a408:	00339713          	slli	a4,t2,0x3
1c00a40c:	00279f93          	slli	t6,a5,0x2
1c00a410:	01e2d393          	srli	t2,t0,0x1e
1c00a414:	01f3efb3          	or	t6,t2,t6
1c00a418:	92ba                	add	t0,t0,a4
1c00a41a:	97fe                	add	a5,a5,t6
1c00a41c:	00e2b733          	sltu	a4,t0,a4
1c00a420:	973e                	add	a4,a4,a5
1c00a422:	00171f93          	slli	t6,a4,0x1
1c00a426:	01f2d793          	srli	a5,t0,0x1f
1c00a42a:	01f7efb3          	or	t6,a5,t6
1c00a42e:	01cfd793          	srli	a5,t6,0x1c
1c00a432:	03078793          	addi	a5,a5,48
1c00a436:	00f30423          	sb	a5,8(t1)
1c00a43a:	00930713          	addi	a4,t1,9
1c00a43e:	ff760793          	addi	a5,a2,-9
1c00a442:	00129393          	slli	t2,t0,0x1
1c00a446:	01efffb3          	and	t6,t6,t5
1c00a44a:	ff7e0413          	addi	s0,t3,-9
1c00a44e:	1e9e2c63          	p.beqimm	t3,9,1c00a646 <pos_libc_prf+0x1a6a>
1c00a452:	1e078a63          	beqz	a5,1c00a646 <pos_libc_prf+0x1a6a>
1c00a456:	01e3d413          	srli	s0,t2,0x1e
1c00a45a:	00329793          	slli	a5,t0,0x3
1c00a45e:	002f9713          	slli	a4,t6,0x2
1c00a462:	007782b3          	add	t0,a5,t2
1c00a466:	8f41                	or	a4,a4,s0
1c00a468:	9fba                	add	t6,t6,a4
1c00a46a:	00f2b7b3          	sltu	a5,t0,a5
1c00a46e:	97fe                	add	a5,a5,t6
1c00a470:	01f2d713          	srli	a4,t0,0x1f
1c00a474:	0786                	slli	a5,a5,0x1
1c00a476:	8fd9                	or	a5,a5,a4
1c00a478:	01c7d713          	srli	a4,a5,0x1c
1c00a47c:	03070713          	addi	a4,a4,48
1c00a480:	00e304a3          	sb	a4,9(t1)
1c00a484:	ff660393          	addi	t2,a2,-10
1c00a488:	00a30713          	addi	a4,t1,10
1c00a48c:	00129f93          	slli	t6,t0,0x1
1c00a490:	01e7f7b3          	and	a5,a5,t5
1c00a494:	ff6e0413          	addi	s0,t3,-10
1c00a498:	1aae2763          	p.beqimm	t3,10,1c00a646 <pos_libc_prf+0x1a6a>
1c00a49c:	1a038563          	beqz	t2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a4a0:	01efd713          	srli	a4,t6,0x1e
1c00a4a4:	002f9f13          	slli	t5,t6,0x2
1c00a4a8:	00279393          	slli	t2,a5,0x2
1c00a4ac:	9ffa                	add	t6,t6,t5
1c00a4ae:	007763b3          	or	t2,a4,t2
1c00a4b2:	01efbf33          	sltu	t5,t6,t5
1c00a4b6:	979e                	add	a5,a5,t2
1c00a4b8:	97fa                	add	a5,a5,t5
1c00a4ba:	01ffd713          	srli	a4,t6,0x1f
1c00a4be:	0786                	slli	a5,a5,0x1
1c00a4c0:	8fd9                	or	a5,a5,a4
1c00a4c2:	01c7d713          	srli	a4,a5,0x1c
1c00a4c6:	03070713          	addi	a4,a4,48
1c00a4ca:	10000f37          	lui	t5,0x10000
1c00a4ce:	1f7d                	addi	t5,t5,-1
1c00a4d0:	00e30523          	sb	a4,10(t1)
1c00a4d4:	ff560913          	addi	s2,a2,-11
1c00a4d8:	00b30713          	addi	a4,t1,11
1c00a4dc:	001f9393          	slli	t2,t6,0x1
1c00a4e0:	01e7f2b3          	and	t0,a5,t5
1c00a4e4:	ff5e0413          	addi	s0,t3,-11
1c00a4e8:	14be2f63          	p.beqimm	t3,11,1c00a646 <pos_libc_prf+0x1a6a>
1c00a4ec:	14090d63          	beqz	s2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a4f0:	003f9713          	slli	a4,t6,0x3
1c00a4f4:	01e3d413          	srli	s0,t2,0x1e
1c00a4f8:	00229f93          	slli	t6,t0,0x2
1c00a4fc:	007707b3          	add	a5,a4,t2
1c00a500:	01f46fb3          	or	t6,s0,t6
1c00a504:	9f96                	add	t6,t6,t0
1c00a506:	00e7b733          	sltu	a4,a5,a4
1c00a50a:	977e                	add	a4,a4,t6
1c00a50c:	01f7d293          	srli	t0,a5,0x1f
1c00a510:	00171f93          	slli	t6,a4,0x1
1c00a514:	01f2efb3          	or	t6,t0,t6
1c00a518:	01cfd713          	srli	a4,t6,0x1c
1c00a51c:	03070713          	addi	a4,a4,48
1c00a520:	00e305a3          	sb	a4,11(t1)
1c00a524:	ff460393          	addi	t2,a2,-12
1c00a528:	00c30713          	addi	a4,t1,12
1c00a52c:	00179293          	slli	t0,a5,0x1
1c00a530:	01efffb3          	and	t6,t6,t5
1c00a534:	ff4e0413          	addi	s0,t3,-12
1c00a538:	10ce2763          	p.beqimm	t3,12,1c00a646 <pos_libc_prf+0x1a6a>
1c00a53c:	10038563          	beqz	t2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a540:	01e2d393          	srli	t2,t0,0x1e
1c00a544:	078e                	slli	a5,a5,0x3
1c00a546:	002f9713          	slli	a4,t6,0x2
1c00a54a:	92be                	add	t0,t0,a5
1c00a54c:	00e3e733          	or	a4,t2,a4
1c00a550:	9fba                	add	t6,t6,a4
1c00a552:	00f2b7b3          	sltu	a5,t0,a5
1c00a556:	97fe                	add	a5,a5,t6
1c00a558:	01f2d713          	srli	a4,t0,0x1f
1c00a55c:	00179f93          	slli	t6,a5,0x1
1c00a560:	01f76fb3          	or	t6,a4,t6
1c00a564:	01cfd793          	srli	a5,t6,0x1c
1c00a568:	03078793          	addi	a5,a5,48
1c00a56c:	00f30623          	sb	a5,12(t1)
1c00a570:	00d30713          	addi	a4,t1,13
1c00a574:	ff360393          	addi	t2,a2,-13
1c00a578:	00129793          	slli	a5,t0,0x1
1c00a57c:	01efffb3          	and	t6,t6,t5
1c00a580:	ff3e0413          	addi	s0,t3,-13
1c00a584:	0cde2163          	p.beqimm	t3,13,1c00a646 <pos_libc_prf+0x1a6a>
1c00a588:	0a038f63          	beqz	t2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a58c:	00329713          	slli	a4,t0,0x3
1c00a590:	01e7d393          	srli	t2,a5,0x1e
1c00a594:	002f9293          	slli	t0,t6,0x2
1c00a598:	97ba                	add	a5,a5,a4
1c00a59a:	0053e2b3          	or	t0,t2,t0
1c00a59e:	9f96                	add	t6,t6,t0
1c00a5a0:	00e7b733          	sltu	a4,a5,a4
1c00a5a4:	977e                	add	a4,a4,t6
1c00a5a6:	01f7d293          	srli	t0,a5,0x1f
1c00a5aa:	00171f93          	slli	t6,a4,0x1
1c00a5ae:	01f2efb3          	or	t6,t0,t6
1c00a5b2:	01cfd713          	srli	a4,t6,0x1c
1c00a5b6:	03070713          	addi	a4,a4,48
1c00a5ba:	00e306a3          	sb	a4,13(t1)
1c00a5be:	1649                	addi	a2,a2,-14
1c00a5c0:	00e30713          	addi	a4,t1,14
1c00a5c4:	00179393          	slli	t2,a5,0x1
1c00a5c8:	01eff2b3          	and	t0,t6,t5
1c00a5cc:	ff2e0413          	addi	s0,t3,-14
1c00a5d0:	06ee2b63          	p.beqimm	t3,14,1c00a646 <pos_libc_prf+0x1a6a>
1c00a5d4:	ca2d                	beqz	a2,1c00a646 <pos_libc_prf+0x1a6a>
1c00a5d6:	078e                	slli	a5,a5,0x3
1c00a5d8:	01e3d413          	srli	s0,t2,0x1e
1c00a5dc:	00229713          	slli	a4,t0,0x2
1c00a5e0:	00778fb3          	add	t6,a5,t2
1c00a5e4:	8f41                	or	a4,a4,s0
1c00a5e6:	00ffb7b3          	sltu	a5,t6,a5
1c00a5ea:	9716                	add	a4,a4,t0
1c00a5ec:	97ba                	add	a5,a5,a4
1c00a5ee:	0786                	slli	a5,a5,0x1
1c00a5f0:	01ffd713          	srli	a4,t6,0x1f
1c00a5f4:	8fd9                	or	a5,a5,a4
1c00a5f6:	01c7d713          	srli	a4,a5,0x1c
1c00a5fa:	03070713          	addi	a4,a4,48
1c00a5fe:	00e30723          	sb	a4,14(t1)
1c00a602:	0f86                	slli	t6,t6,0x1
1c00a604:	00f30713          	addi	a4,t1,15
1c00a608:	01e7ff33          	and	t5,a5,t5
1c00a60c:	ff1e0413          	addi	s0,t3,-15
1c00a610:	02fe2b63          	p.beqimm	t3,15,1c00a646 <pos_libc_prf+0x1a6a>
1c00a614:	02162963          	p.beqimm	a2,1,1c00a646 <pos_libc_prf+0x1a6a>
1c00a618:	002f9613          	slli	a2,t6,0x2
1c00a61c:	01efd793          	srli	a5,t6,0x1e
1c00a620:	002f1713          	slli	a4,t5,0x2
1c00a624:	8f5d                	or	a4,a4,a5
1c00a626:	01f607b3          	add	a5,a2,t6
1c00a62a:	977a                	add	a4,a4,t5
1c00a62c:	00c7b7b3          	sltu	a5,a5,a2
1c00a630:	97ba                	add	a5,a5,a4
1c00a632:	0786                	slli	a5,a5,0x1
1c00a634:	83f1                	srli	a5,a5,0x1c
1c00a636:	03078793          	addi	a5,a5,48
1c00a63a:	01030713          	addi	a4,t1,16
1c00a63e:	00f307a3          	sb	a5,15(t1)
1c00a642:	ff0e0413          	addi	s0,t3,-16
1c00a646:	a2059663          	bnez	a1,1c009872 <pos_libc_prf+0xc96>
1c00a64a:	9ea2                	add	t4,t4,s0
1c00a64c:	a4eff06f          	j	1c00989a <pos_libc_prf+0xcbe>
1c00a650:	8ea2                	mv	t4,s0
1c00a652:	d402                	sw	zero,40(sp)
1c00a654:	cc02                	sw	zero,24(sp)
1c00a656:	a38ff06f          	j	1c00988e <pos_libc_prf+0xcb2>
1c00a65a:	47e2                	lw	a5,24(sp)
1c00a65c:	00778713          	addi	a4,a5,7
1c00a660:	c4073733          	p.bclr	a4,a4,2,0
1c00a664:	8d3a                	mv	s10,a4
1c00a666:	4358                	lw	a4,4(a4)
1c00a668:	008d278b          	p.lw	a5,8(s10!)
1c00a66c:	d43a                	sw	a4,40(sp)
1c00a66e:	86ba                	mv	a3,a4
1c00a670:	871fe06f          	j	1c008ee0 <pos_libc_prf+0x304>
1c00a674:	47e2                	lw	a5,24(sp)
1c00a676:	079d                	addi	a5,a5,7
1c00a678:	c407b7b3          	p.bclr	a5,a5,2,0
1c00a67c:	8d3e                	mv	s10,a5
1c00a67e:	008d280b          	p.lw	a6,8(s10!)
1c00a682:	0047aa83          	lw	s5,4(a5)
1c00a686:	a0ffe06f          	j	1c009094 <pos_libc_prf+0x4b8>
1c00a68a:	4e81                	li	t4,0
1c00a68c:	4581                	li	a1,0
1c00a68e:	cc3fe06f          	j	1c009350 <pos_libc_prf+0x774>
1c00a692:	c822                	sw	s0,16(sp)
1c00a694:	03410913          	addi	s2,sp,52
1c00a698:	8456                	mv	s0,s5
1c00a69a:	4ba5                	li	s7,9
1c00a69c:	8ac2                	mv	s5,a6
1c00a69e:	4629                	li	a2,10
1c00a6a0:	4681                	li	a3,0
1c00a6a2:	8556                	mv	a0,s5
1c00a6a4:	85a2                	mv	a1,s0
1c00a6a6:	d37fd0ef          	jal	ra,1c0083dc <__umoddi3>
1c00a6aa:	89aa                	mv	s3,a0
1c00a6ac:	85a2                	mv	a1,s0
1c00a6ae:	8556                	mv	a0,s5
1c00a6b0:	4629                	li	a2,10
1c00a6b2:	4681                	li	a3,0
1c00a6b4:	9e9fd0ef          	jal	ra,1c00809c <__udivdi3>
1c00a6b8:	03098993          	addi	s3,s3,48
1c00a6bc:	874a                	mv	a4,s2
1c00a6be:	86a2                	mv	a3,s0
1c00a6c0:	013900ab          	p.sb	s3,1(s2!)
1c00a6c4:	8656                	mv	a2,s5
1c00a6c6:	842e                	mv	s0,a1
1c00a6c8:	8aaa                	mv	s5,a0
1c00a6ca:	faf1                	bnez	a3,1c00a69e <pos_libc_prf+0x1ac2>
1c00a6cc:	fccbe9e3          	bltu	s7,a2,1c00a69e <pos_libc_prf+0x1ac2>
1c00a6d0:	185c                	addi	a5,sp,52
1c00a6d2:	00090023          	sb	zero,0(s2)
1c00a6d6:	4442                	lw	s0,16(sp)
1c00a6d8:	40f909b3          	sub	s3,s2,a5
1c00a6dc:	0ae7f763          	bgeu	a5,a4,1c00a78a <pos_libc_prf+0x1bae>
1c00a6e0:	5792                	lw	a5,36(sp)
1c00a6e2:	1854                	addi	a3,sp,52
1c00a6e4:	00e78633          	add	a2,a5,a4
1c00a6e8:	00165793          	srli	a5,a2,0x1
1c00a6ec:	00178313          	addi	t1,a5,1
1c00a6f0:	969a                	add	a3,a3,t1
1c00a6f2:	40f707b3          	sub	a5,a4,a5
1c00a6f6:	04f6b6b3          	p.sletu	a3,a3,a5
1c00a6fa:	185c                	addi	a5,sp,52
1c00a6fc:	04f937b3          	p.sletu	a5,s2,a5
1c00a700:	8fd5                	or	a5,a5,a3
1c00a702:	86ba                	mv	a3,a4
1c00a704:	34078463          	beqz	a5,1c00aa4c <pos_libc_prf+0x1e70>
1c00a708:	00663613          	sltiu	a2,a2,6
1c00a70c:	34061063          	bnez	a2,1c00aa4c <pos_libc_prf+0x1e70>
1c00a710:	c2033633          	p.bclr	a2,t1,1,0
1c00a714:	1671                	addi	a2,a2,-4
1c00a716:	8209                	srli	a2,a2,0x2
1c00a718:	ffd70513          	addi	a0,a4,-3
1c00a71c:	184c                	addi	a1,sp,52
1c00a71e:	0605                	addi	a2,a2,1
1c00a720:	00a640fb          	lp.setup	x1,a2,1c00a734 <pos_libc_prf+0x1b58>
1c00a724:	4194                	lw	a3,0(a1)
1c00a726:	411c                	lw	a5,0(a0)
1c00a728:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c00a72c:	fed52e2b          	p.sw	a3,-4(a0!)
1c00a730:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c00a734:	00f5a22b          	p.sw	a5,4(a1!)
1c00a738:	c20336b3          	p.bclr	a3,t1,1,0
1c00a73c:	185c                	addi	a5,sp,52
1c00a73e:	8f15                	sub	a4,a4,a3
1c00a740:	97b6                	add	a5,a5,a3
1c00a742:	04668463          	beq	a3,t1,1c00a78a <pos_libc_prf+0x1bae>
1c00a746:	0007c603          	lbu	a2,0(a5)
1c00a74a:	00074583          	lbu	a1,0(a4)
1c00a74e:	86ba                	mv	a3,a4
1c00a750:	fec68fab          	p.sb	a2,-1(a3!)
1c00a754:	863e                	mv	a2,a5
1c00a756:	00b600ab          	p.sb	a1,1(a2!)
1c00a75a:	02d67863          	bgeu	a2,a3,1c00a78a <pos_libc_prf+0x1bae>
1c00a75e:	0017c583          	lbu	a1,1(a5)
1c00a762:	fff74603          	lbu	a2,-1(a4)
1c00a766:	00278693          	addi	a3,a5,2
1c00a76a:	feb70fa3          	sb	a1,-1(a4)
1c00a76e:	00c780a3          	sb	a2,1(a5)
1c00a772:	ffe70613          	addi	a2,a4,-2
1c00a776:	00c6fa63          	bgeu	a3,a2,1c00a78a <pos_libc_prf+0x1bae>
1c00a77a:	0027c603          	lbu	a2,2(a5)
1c00a77e:	ffe74683          	lbu	a3,-2(a4)
1c00a782:	fec70f23          	sb	a2,-2(a4)
1c00a786:	00d78123          	sb	a3,2(a5)
1c00a78a:	c802                	sw	zero,16(sp)
1c00a78c:	875fe06f          	j	1c009000 <pos_libc_prf+0x424>
1c00a790:	8f49                	or	a4,a4,a0
1c00a792:	fbf68693          	addi	a3,a3,-65
1c00a796:	00330993          	addi	s3,t1,3
1c00a79a:	47e5                	li	a5,25
1c00a79c:	20071a63          	bnez	a4,1c00a9b0 <pos_libc_prf+0x1dd4>
1c00a7a0:	2ed7e563          	bltu	a5,a3,1c00aa8a <pos_libc_prf+0x1eae>
1c00a7a4:	04900793          	li	a5,73
1c00a7a8:	00f30023          	sb	a5,0(t1)
1c00a7ac:	04e00793          	li	a5,78
1c00a7b0:	00f300a3          	sb	a5,1(t1)
1c00a7b4:	04600793          	li	a5,70
1c00a7b8:	00f30123          	sb	a5,2(t1)
1c00a7bc:	185c                	addi	a5,sp,52
1c00a7be:	000301a3          	sb	zero,3(t1)
1c00a7c2:	40f989b3          	sub	s3,s3,a5
1c00a7c6:	4401                	li	s0,0
1c00a7c8:	d402                	sw	zero,40(sp)
1c00a7ca:	cc02                	sw	zero,24(sp)
1c00a7cc:	8daff06f          	j	1c0098a6 <pos_libc_prf+0xcca>
1c00a7d0:	1854                	addi	a3,sp,52
1c00a7d2:	45e5                	li	a1,25
1c00a7d4:	0016c70b          	p.lbu	a4,1(a3!)
1c00a7d8:	f9f70793          	addi	a5,a4,-97
1c00a7dc:	0ff7f793          	andi	a5,a5,255
1c00a7e0:	fe070613          	addi	a2,a4,-32
1c00a7e4:	00f5ee63          	bltu	a1,a5,1c00a800 <pos_libc_prf+0x1c24>
1c00a7e8:	fec68fa3          	sb	a2,-1(a3)
1c00a7ec:	0016c70b          	p.lbu	a4,1(a3!)
1c00a7f0:	f9f70793          	addi	a5,a4,-97
1c00a7f4:	0ff7f793          	andi	a5,a5,255
1c00a7f8:	fe070613          	addi	a2,a4,-32
1c00a7fc:	fef5f6e3          	bgeu	a1,a5,1c00a7e8 <pos_libc_prf+0x1c0c>
1c00a800:	fb71                	bnez	a4,1c00a7d4 <pos_libc_prf+0x1bf8>
1c00a802:	9b7fe06f          	j	1c0091b8 <pos_libc_prf+0x5dc>
1c00a806:	c0260613          	addi	a2,a2,-1022
1c00a80a:	57f9                	li	a5,-2
1c00a80c:	8eaa                	mv	t4,a0
1c00a80e:	c1f74f33          	p.bset	t5,a4,0,31
1c00a812:	4501                	li	a0,0
1c00a814:	98f64063          	blt	a2,a5,1c009994 <pos_libc_prf+0xdb8>
1c00a818:	9e8ff06f          	j	1c009a00 <pos_libc_prf+0xe24>
1c00a81c:	57b2                	lw	a5,44(sp)
1c00a81e:	1e078963          	beqz	a5,1c00aa10 <pos_libc_prf+0x1e34>
1c00a822:	03000793          	li	a5,48
1c00a826:	02f10a23          	sb	a5,52(sp)
1c00a82a:	015867b3          	or	a5,a6,s5
1c00a82e:	03510513          	addi	a0,sp,53
1c00a832:	24078663          	beqz	a5,1c00aa7e <pos_libc_prf+0x1ea2>
1c00a836:	89aa                	mv	s3,a0
1c00a838:	459d                	li	a1,7
1c00a83a:	f8383733          	p.bclr	a4,a6,28,3
1c00a83e:	03070713          	addi	a4,a4,48
1c00a842:	01da9793          	slli	a5,s5,0x1d
1c00a846:	86d6                	mv	a3,s5
1c00a848:	00385613          	srli	a2,a6,0x3
1c00a84c:	834e                	mv	t1,s3
1c00a84e:	00e980ab          	p.sb	a4,1(s3!)
1c00a852:	003ada93          	srli	s5,s5,0x3
1c00a856:	8742                	mv	a4,a6
1c00a858:	00c7e833          	or	a6,a5,a2
1c00a85c:	fef9                	bnez	a3,1c00a83a <pos_libc_prf+0x1c5e>
1c00a85e:	fce5eee3          	bltu	a1,a4,1c00a83a <pos_libc_prf+0x1c5e>
1c00a862:	00098023          	sb	zero,0(s3)
1c00a866:	0a657363          	bgeu	a0,t1,1c00a90c <pos_libc_prf+0x1d30>
1c00a86a:	fff54713          	not	a4,a0
1c00a86e:	971a                	add	a4,a4,t1
1c00a870:	00175793          	srli	a5,a4,0x1
1c00a874:	00178e13          	addi	t3,a5,1
1c00a878:	01c506b3          	add	a3,a0,t3
1c00a87c:	40f307b3          	sub	a5,t1,a5
1c00a880:	04f6b7b3          	p.sletu	a5,a3,a5
1c00a884:	04a9b6b3          	p.sletu	a3,s3,a0
1c00a888:	8fd5                	or	a5,a5,a3
1c00a88a:	85aa                	mv	a1,a0
1c00a88c:	869a                	mv	a3,t1
1c00a88e:	c7c9                	beqz	a5,1c00a918 <pos_libc_prf+0x1d3c>
1c00a890:	00673713          	sltiu	a4,a4,6
1c00a894:	e351                	bnez	a4,1c00a918 <pos_libc_prf+0x1d3c>
1c00a896:	c20e3733          	p.bclr	a4,t3,1,0
1c00a89a:	ffd30793          	addi	a5,t1,-3
1c00a89e:	972a                	add	a4,a4,a0
1c00a8a0:	4190                	lw	a2,0(a1)
1c00a8a2:	4394                	lw	a3,0(a5)
1c00a8a4:	c2d67657          	pv.shufflei0.sci.b	a2,a2,27
1c00a8a8:	fec7ae2b          	p.sw	a2,-4(a5!)
1c00a8ac:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c00a8b0:	00d5a22b          	p.sw	a3,4(a1!)
1c00a8b4:	fee596e3          	bne	a1,a4,1c00a8a0 <pos_libc_prf+0x1cc4>
1c00a8b8:	c20e36b3          	p.bclr	a3,t3,1,0
1c00a8bc:	40d30733          	sub	a4,t1,a3
1c00a8c0:	00d507b3          	add	a5,a0,a3
1c00a8c4:	05c68463          	beq	a3,t3,1c00a90c <pos_libc_prf+0x1d30>
1c00a8c8:	0007c603          	lbu	a2,0(a5)
1c00a8cc:	00074583          	lbu	a1,0(a4)
1c00a8d0:	86ba                	mv	a3,a4
1c00a8d2:	fec68fab          	p.sb	a2,-1(a3!)
1c00a8d6:	863e                	mv	a2,a5
1c00a8d8:	00b600ab          	p.sb	a1,1(a2!)
1c00a8dc:	02d67863          	bgeu	a2,a3,1c00a90c <pos_libc_prf+0x1d30>
1c00a8e0:	0017c583          	lbu	a1,1(a5)
1c00a8e4:	fff74603          	lbu	a2,-1(a4)
1c00a8e8:	00278693          	addi	a3,a5,2
1c00a8ec:	feb70fa3          	sb	a1,-1(a4)
1c00a8f0:	00c780a3          	sb	a2,1(a5)
1c00a8f4:	ffe70613          	addi	a2,a4,-2
1c00a8f8:	00c6fa63          	bgeu	a3,a2,1c00a90c <pos_libc_prf+0x1d30>
1c00a8fc:	0027c603          	lbu	a2,2(a5)
1c00a900:	ffe74683          	lbu	a3,-2(a4)
1c00a904:	fec70f23          	sb	a2,-2(a4)
1c00a908:	00d78123          	sb	a3,2(a5)
1c00a90c:	185c                	addi	a5,sp,52
1c00a90e:	40f989b3          	sub	s3,s3,a5
1c00a912:	c802                	sw	zero,16(sp)
1c00a914:	eecfe06f          	j	1c009000 <pos_libc_prf+0x424>
1c00a918:	00054703          	lbu	a4,0(a0)
1c00a91c:	0006c783          	lbu	a5,0(a3)
1c00a920:	fee68fab          	p.sb	a4,-1(a3!)
1c00a924:	00f500ab          	p.sb	a5,1(a0!)
1c00a928:	fed568e3          	bltu	a0,a3,1c00a918 <pos_libc_prf+0x1d3c>
1c00a92c:	b7c5                	j	1c00a90c <pos_libc_prf+0x1d30>
1c00a92e:	03610793          	addi	a5,sp,54
1c00a932:	0007c603          	lbu	a2,0(a5)
1c00a936:	00074683          	lbu	a3,0(a4)
1c00a93a:	fec70fab          	p.sb	a2,-1(a4!)
1c00a93e:	00d780ab          	p.sb	a3,1(a5!)
1c00a942:	fee7e8e3          	bltu	a5,a4,1c00a932 <pos_libc_prf+0x1d56>
1c00a946:	a38ff06f          	j	1c009b7e <pos_libc_prf+0xfa2>
1c00a94a:	01670023          	sb	s6,0(a4)
1c00a94e:	ae8fe06f          	j	1c008c36 <pos_libc_prf+0x5a>
1c00a952:	87c2                	mv	a5,a6
1c00a954:	0007c603          	lbu	a2,0(a5)
1c00a958:	00074683          	lbu	a3,0(a4)
1c00a95c:	fec70fab          	p.sb	a2,-1(a4!)
1c00a960:	00d780ab          	p.sb	a3,1(a5!)
1c00a964:	fee7e8e3          	bltu	a5,a4,1c00a954 <pos_libc_prf+0x1d78>
1c00a968:	e80fe06f          	j	1c008fe8 <pos_libc_prf+0x40c>
1c00a96c:	4be2                	lw	s7,24(sp)
1c00a96e:	a039                	j	1c00a97c <pos_libc_prf+0x1da0>
1c00a970:	9a02                	jalr	s4
1c00a972:	1bfd                	addi	s7,s7,-1
1c00a974:	01f53463          	p.bneimm	a0,-1,1c00a97c <pos_libc_prf+0x1da0>
1c00a978:	c44fe06f          	j	1c008dbc <pos_libc_prf+0x1e0>
1c00a97c:	85a6                	mv	a1,s1
1c00a97e:	03000513          	li	a0,48
1c00a982:	ff7047e3          	bgtz	s7,1c00a970 <pos_libc_prf+0x1d94>
1c00a986:	47e2                	lw	a5,24(sp)
1c00a988:	40fc0c33          	sub	s8,s8,a5
1c00a98c:	cfefe06f          	j	1c008e8a <pos_libc_prf+0x2ae>
1c00a990:	00a407b3          	add	a5,s0,a0
1c00a994:	4581                	li	a1,0
1c00a996:	a96ff06f          	j	1c009c2c <pos_libc_prf+0x1050>
1c00a99a:	1858                	addi	a4,sp,52
1c00a99c:	cc3a                	sw	a4,24(sp)
1c00a99e:	893a                	mv	s2,a4
1c00a9a0:	d5cfe06f          	j	1c008efc <pos_libc_prf+0x320>
1c00a9a4:	896e                	mv	s2,s11
1c00a9a6:	ca02                	sw	zero,20(sp)
1c00a9a8:	c802                	sw	zero,16(sp)
1c00a9aa:	4981                	li	s3,0
1c00a9ac:	c7afe06f          	j	1c008e26 <pos_libc_prf+0x24a>
1c00a9b0:	0ad7ec63          	bltu	a5,a3,1c00aa68 <pos_libc_prf+0x1e8c>
1c00a9b4:	04e00793          	li	a5,78
1c00a9b8:	04100713          	li	a4,65
1c00a9bc:	00f30023          	sb	a5,0(t1)
1c00a9c0:	00e300a3          	sb	a4,1(t1)
1c00a9c4:	00f30123          	sb	a5,2(t1)
1c00a9c8:	bbd5                	j	1c00a7bc <pos_libc_prf+0x1be0>
1c00a9ca:	06600693          	li	a3,102
1c00a9ce:	4781                	li	a5,0
1c00a9d0:	080002b7          	lui	t0,0x8000
1c00a9d4:	9effe06f          	j	1c0093c2 <pos_libc_prf+0x7e6>
1c00a9d8:	03000613          	li	a2,48
1c00a9dc:	00c30023          	sb	a2,0(t1)
1c00a9e0:	5632                	lw	a2,44(sp)
1c00a9e2:	e239                	bnez	a2,1c00aa28 <pos_libc_prf+0x1e4c>
1c00a9e4:	c061                	beqz	s0,1c00aaa4 <pos_libc_prf+0x1ec8>
1c00a9e6:	02e00713          	li	a4,46
1c00a9ea:	00e300a3          	sb	a4,1(t1)
1c00a9ee:	0309                	addi	t1,t1,2
1c00a9f0:	14050c63          	beqz	a0,1c00ab48 <pos_libc_prf+0x1f6c>
1c00a9f4:	40a00733          	neg	a4,a0
1c00a9f8:	04874eb3          	p.min	t4,a4,s0
1c00a9fc:	d476                	sw	t4,40(sp)
1c00a9fe:	41d40e33          	sub	t3,s0,t4
1c00aa02:	4641                	li	a2,16
1c00aa04:	cc02                	sw	zero,24(sp)
1c00aa06:	fbc04163          	bgtz	t3,1c00a1a8 <pos_libc_prf+0x15cc>
1c00aa0a:	8472                	mv	s0,t3
1c00aa0c:	871a                	mv	a4,t1
1c00aa0e:	b925                	j	1c00a646 <pos_libc_prf+0x1a6a>
1c00aa10:	1848                	addi	a0,sp,52
1c00aa12:	b515                	j	1c00a836 <pos_libc_prf+0x1c5a>
1c00aa14:	833a                	mv	t1,a4
1c00aa16:	02e00713          	li	a4,46
1c00aa1a:	4ee2                	lw	t4,24(sp)
1c00aa1c:	00e300ab          	p.sb	a4,1(t1!)
1c00aa20:	8e22                	mv	t3,s0
1c00aa22:	4501                	li	a0,0
1c00aa24:	d402                	sw	zero,40(sp)
1c00aa26:	b7c5                	j	1c00aa06 <pos_libc_prf+0x1e2a>
1c00aa28:	02e00713          	li	a4,46
1c00aa2c:	00e300a3          	sb	a4,1(t1)
1c00aa30:	0309                	addi	t1,t1,2
1c00aa32:	14050363          	beqz	a0,1c00ab78 <pos_libc_prf+0x1f9c>
1c00aa36:	fc5d                	bnez	s0,1c00a9f4 <pos_libc_prf+0x1e18>
1c00aa38:	cc02                	sw	zero,24(sp)
1c00aa3a:	4e81                	li	t4,0
1c00aa3c:	d402                	sw	zero,40(sp)
1c00aa3e:	871a                	mv	a4,t1
1c00aa40:	b119                	j	1c00a646 <pos_libc_prf+0x1a6a>
1c00aa42:	87a2                	mv	a5,s0
1c00aa44:	4581                	li	a1,0
1c00aa46:	8436                	mv	s0,a3
1c00aa48:	9e4ff06f          	j	1c009c2c <pos_libc_prf+0x1050>
1c00aa4c:	185c                	addi	a5,sp,52
1c00aa4e:	0007c603          	lbu	a2,0(a5)
1c00aa52:	0006c703          	lbu	a4,0(a3)
1c00aa56:	fec68fab          	p.sb	a2,-1(a3!)
1c00aa5a:	00e780ab          	p.sb	a4,1(a5!)
1c00aa5e:	fed7e8e3          	bltu	a5,a3,1c00aa4e <pos_libc_prf+0x1e72>
1c00aa62:	c802                	sw	zero,16(sp)
1c00aa64:	d9cfe06f          	j	1c009000 <pos_libc_prf+0x424>
1c00aa68:	06e00793          	li	a5,110
1c00aa6c:	06100713          	li	a4,97
1c00aa70:	00f30023          	sb	a5,0(t1)
1c00aa74:	00e300a3          	sb	a4,1(t1)
1c00aa78:	00f30123          	sb	a5,2(t1)
1c00aa7c:	b381                	j	1c00a7bc <pos_libc_prf+0x1be0>
1c00aa7e:	02010aa3          	sb	zero,53(sp)
1c00aa82:	c802                	sw	zero,16(sp)
1c00aa84:	4985                	li	s3,1
1c00aa86:	d7afe06f          	j	1c009000 <pos_libc_prf+0x424>
1c00aa8a:	06900793          	li	a5,105
1c00aa8e:	00f30023          	sb	a5,0(t1)
1c00aa92:	06e00793          	li	a5,110
1c00aa96:	00f300a3          	sb	a5,1(t1)
1c00aa9a:	06600793          	li	a5,102
1c00aa9e:	00f30123          	sb	a5,2(t1)
1c00aaa2:	bb29                	j	1c00a7bc <pos_libc_prf+0x1be0>
1c00aaa4:	cc02                	sw	zero,24(sp)
1c00aaa6:	c509                	beqz	a0,1c00aab0 <pos_libc_prf+0x1ed4>
1c00aaa8:	4e81                	li	t4,0
1c00aaaa:	d402                	sw	zero,40(sp)
1c00aaac:	be69                	j	1c00a646 <pos_libc_prf+0x1a6a>
1c00aaae:	4562                	lw	a0,24(sp)
1c00aab0:	4ee2                	lw	t4,24(sp)
1c00aab2:	d402                	sw	zero,40(sp)
1c00aab4:	cc2a                	sw	a0,24(sp)
1c00aab6:	4401                	li	s0,0
1c00aab8:	4501                	li	a0,0
1c00aaba:	b671                	j	1c00a646 <pos_libc_prf+0x1a6a>
1c00aabc:	00860713          	addi	a4,a2,8
1c00aac0:	8472                	mv	s0,t3
1c00aac2:	da7fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aac6:	00760713          	addi	a4,a2,7
1c00aaca:	843e                	mv	s0,a5
1c00aacc:	d9dfe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aad0:	00660713          	addi	a4,a2,6
1c00aad4:	8472                	mv	s0,t3
1c00aad6:	d93fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aada:	00560713          	addi	a4,a2,5
1c00aade:	843e                	mv	s0,a5
1c00aae0:	d89fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aae4:	00460713          	addi	a4,a2,4
1c00aae8:	8476                	mv	s0,t4
1c00aaea:	d7ffe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aaee:	00360713          	addi	a4,a2,3
1c00aaf2:	847a                	mv	s0,t5
1c00aaf4:	d75fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00aaf8:	00260713          	addi	a4,a2,2
1c00aafc:	4401                	li	s0,0
1c00aafe:	d6bfe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab02:	00160713          	addi	a4,a2,1
1c00ab06:	4401                	li	s0,0
1c00ab08:	d61fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab0c:	00c60713          	addi	a4,a2,12
1c00ab10:	8472                	mv	s0,t3
1c00ab12:	d57fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab16:	00b60713          	addi	a4,a2,11
1c00ab1a:	8472                	mv	s0,t3
1c00ab1c:	d4dfe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab20:	00a60713          	addi	a4,a2,10
1c00ab24:	8472                	mv	s0,t3
1c00ab26:	d43fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab2a:	00960713          	addi	a4,a2,9
1c00ab2e:	843e                	mv	s0,a5
1c00ab30:	d39fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab34:	00e60713          	addi	a4,a2,14
1c00ab38:	8476                	mv	s0,t4
1c00ab3a:	d2ffe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab3e:	00d60713          	addi	a4,a2,13
1c00ab42:	843e                	mv	s0,a5
1c00ab44:	d25fe06f          	j	1c009868 <pos_libc_prf+0xc8c>
1c00ab48:	cc02                	sw	zero,24(sp)
1c00ab4a:	4641                	li	a2,16
1c00ab4c:	e54ff06f          	j	1c00a1a0 <pos_libc_prf+0x15c4>
1c00ab50:	00f30713          	addi	a4,t1,15
1c00ab54:	4605                	li	a2,1
1c00ab56:	e36ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab5a:	00b30713          	addi	a4,t1,11
1c00ab5e:	4615                	li	a2,5
1c00ab60:	e2cff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab64:	00a30713          	addi	a4,t1,10
1c00ab68:	4619                	li	a2,6
1c00ab6a:	e22ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab6e:	00930713          	addi	a4,t1,9
1c00ab72:	461d                	li	a2,7
1c00ab74:	e18ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab78:	cc02                	sw	zero,24(sp)
1c00ab7a:	d402                	sw	zero,40(sp)
1c00ab7c:	4e81                	li	t4,0
1c00ab7e:	8e22                	mv	t3,s0
1c00ab80:	4641                	li	a2,16
1c00ab82:	e2804563          	bgtz	s0,1c00a1ac <pos_libc_prf+0x15d0>
1c00ab86:	b551                	j	1c00aa0a <pos_libc_prf+0x1e2e>
1c00ab88:	00e30713          	addi	a4,t1,14
1c00ab8c:	4609                	li	a2,2
1c00ab8e:	dfeff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab92:	00d30713          	addi	a4,t1,13
1c00ab96:	460d                	li	a2,3
1c00ab98:	df4ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00ab9c:	00830713          	addi	a4,t1,8
1c00aba0:	4621                	li	a2,8
1c00aba2:	deaff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00aba6:	00730713          	addi	a4,t1,7
1c00abaa:	4625                	li	a2,9
1c00abac:	de0ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abb0:	00630713          	addi	a4,t1,6
1c00abb4:	4629                	li	a2,10
1c00abb6:	dd6ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abba:	00530713          	addi	a4,t1,5
1c00abbe:	462d                	li	a2,11
1c00abc0:	dccff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abc4:	00430713          	addi	a4,t1,4
1c00abc8:	4631                	li	a2,12
1c00abca:	dc2ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abce:	00330713          	addi	a4,t1,3
1c00abd2:	4635                	li	a2,13
1c00abd4:	db8ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abd8:	00230713          	addi	a4,t1,2
1c00abdc:	4639                	li	a2,14
1c00abde:	cc02                	sw	zero,24(sp)
1c00abe0:	dacff06f          	j	1c00a18c <pos_libc_prf+0x15b0>
1c00abe4:	463d                	li	a2,15
1c00abe6:	cc02                	sw	zero,24(sp)
1c00abe8:	da4ff06f          	j	1c00a18c <pos_libc_prf+0x15b0>

1c00abec <pos_init_start>:
1c00abec:	1141                	addi	sp,sp,-16
1c00abee:	c422                	sw	s0,8(sp)
1c00abf0:	1c001437          	lui	s0,0x1c001
1c00abf4:	c606                	sw	ra,12(sp)
1c00abf6:	84840413          	addi	s0,s0,-1976 # 1c000848 <ctor_list>
1c00abfa:	22f5                	jal	1c00ade6 <pos_irq_init>
1c00abfc:	d2bfd0ef          	jal	ra,1c008926 <pos_soc_init>
1c00ac00:	2c21                	jal	1c00ae18 <pos_soc_event_init>
1c00ac02:	2a15                	jal	1c00ad36 <pos_allocs_init>
1c00ac04:	284d                	jal	1c00acb6 <pos_sched_init>
1c00ac06:	405c                	lw	a5,4(s0)
1c00ac08:	c791                	beqz	a5,1c00ac14 <pos_init_start+0x28>
1c00ac0a:	0421                	addi	s0,s0,8
1c00ac0c:	9782                	jalr	a5
1c00ac0e:	0044278b          	p.lw	a5,4(s0!)
1c00ac12:	ffed                	bnez	a5,1c00ac0c <pos_init_start+0x20>
1c00ac14:	f2ffd0ef          	jal	ra,1c008b42 <pos_io_start>
1c00ac18:	300467f3          	csrrsi	a5,mstatus,8
1c00ac1c:	40b2                	lw	ra,12(sp)
1c00ac1e:	4422                	lw	s0,8(sp)
1c00ac20:	0141                	addi	sp,sp,16
1c00ac22:	8082                	ret

1c00ac24 <pos_init_stop>:
1c00ac24:	1141                	addi	sp,sp,-16
1c00ac26:	c422                	sw	s0,8(sp)
1c00ac28:	1c001437          	lui	s0,0x1c001
1c00ac2c:	c606                	sw	ra,12(sp)
1c00ac2e:	86040413          	addi	s0,s0,-1952 # 1c000860 <dtor_list>
1c00ac32:	f57fd0ef          	jal	ra,1c008b88 <pos_io_stop>
1c00ac36:	405c                	lw	a5,4(s0)
1c00ac38:	c791                	beqz	a5,1c00ac44 <pos_init_stop+0x20>
1c00ac3a:	0421                	addi	s0,s0,8
1c00ac3c:	9782                	jalr	a5
1c00ac3e:	0044278b          	p.lw	a5,4(s0!)
1c00ac42:	ffed                	bnez	a5,1c00ac3c <pos_init_stop+0x18>
1c00ac44:	40b2                	lw	ra,12(sp)
1c00ac46:	4422                	lw	s0,8(sp)
1c00ac48:	0141                	addi	sp,sp,16
1c00ac4a:	8082                	ret

1c00ac4c <pos_cbsys_add>:
1c00ac4c:	00259793          	slli	a5,a1,0x2
1c00ac50:	1c0015b7          	lui	a1,0x1c001
1c00ac54:	21058593          	addi	a1,a1,528 # 1c001210 <pos_cbsys_first>
1c00ac58:	95be                	add	a1,a1,a5
1c00ac5a:	419c                	lw	a5,0(a1)
1c00ac5c:	c110                	sw	a2,0(a0)
1c00ac5e:	c154                	sw	a3,4(a0)
1c00ac60:	c51c                	sw	a5,8(a0)
1c00ac62:	c188                	sw	a0,0(a1)
1c00ac64:	8082                	ret

1c00ac66 <pi_open_from_conf>:
1c00ac66:	c14c                	sw	a1,4(a0)
1c00ac68:	8082                	ret

1c00ac6a <pos_task_handle_blocking>:
1c00ac6a:	4785                	li	a5,1
1c00ac6c:	00f50a23          	sb	a5,20(a0)
1c00ac70:	8082                	ret

1c00ac72 <pos_task_handle>:
1c00ac72:	1141                	addi	sp,sp,-16
1c00ac74:	c422                	sw	s0,8(sp)
1c00ac76:	01c00413          	li	s0,28
1c00ac7a:	401c                	lw	a5,0(s0)
1c00ac7c:	c606                	sw	ra,12(sp)
1c00ac7e:	c385                	beqz	a5,1c00ac9e <pos_task_handle+0x2c>
1c00ac80:	4394                	lw	a3,0(a5)
1c00ac82:	43d8                	lw	a4,4(a5)
1c00ac84:	4788                	lw	a0,8(a5)
1c00ac86:	c014                	sw	a3,0(s0)
1c00ac88:	300467f3          	csrrsi	a5,mstatus,8
1c00ac8c:	9702                	jalr	a4
1c00ac8e:	300477f3          	csrrci	a5,mstatus,8
1c00ac92:	401c                	lw	a5,0(s0)
1c00ac94:	f7f5                	bnez	a5,1c00ac80 <pos_task_handle+0xe>
1c00ac96:	40b2                	lw	ra,12(sp)
1c00ac98:	4422                	lw	s0,8(sp)
1c00ac9a:	0141                	addi	sp,sp,16
1c00ac9c:	8082                	ret
1c00ac9e:	10500073          	wfi
1c00aca2:	300467f3          	csrrsi	a5,mstatus,8
1c00aca6:	300477f3          	csrrci	a5,mstatus,8
1c00acaa:	401c                	lw	a5,0(s0)
1c00acac:	fbf1                	bnez	a5,1c00ac80 <pos_task_handle+0xe>
1c00acae:	40b2                	lw	ra,12(sp)
1c00acb0:	4422                	lw	s0,8(sp)
1c00acb2:	0141                	addi	sp,sp,16
1c00acb4:	8082                	ret

1c00acb6 <pos_sched_init>:
1c00acb6:	00002e23          	sw	zero,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00acba:	8082                	ret

1c00acbc <pos_alloc_init>:
1c00acbc:	00758793          	addi	a5,a1,7
1c00acc0:	c407b7b3          	p.bclr	a5,a5,2,0
1c00acc4:	40b785b3          	sub	a1,a5,a1
1c00acc8:	c11c                	sw	a5,0(a0)
1c00acca:	40b605b3          	sub	a1,a2,a1
1c00acce:	00b05763          	blez	a1,1c00acdc <pos_alloc_init+0x20>
1c00acd2:	c405b5b3          	p.bclr	a1,a1,2,0
1c00acd6:	c38c                	sw	a1,0(a5)
1c00acd8:	0007a223          	sw	zero,4(a5)
1c00acdc:	8082                	ret

1c00acde <pos_alloc>:
1c00acde:	411c                	lw	a5,0(a0)
1c00ace0:	00758713          	addi	a4,a1,7
1c00ace4:	862a                	mv	a2,a0
1c00ace6:	c40735b3          	p.bclr	a1,a4,2,0
1c00acea:	4681                	li	a3,0
1c00acec:	e789                	bnez	a5,1c00acf6 <pos_alloc+0x18>
1c00acee:	a091                	j	1c00ad32 <pos_alloc+0x54>
1c00acf0:	86be                	mv	a3,a5
1c00acf2:	c505                	beqz	a0,1c00ad1a <pos_alloc+0x3c>
1c00acf4:	87aa                	mv	a5,a0
1c00acf6:	4398                	lw	a4,0(a5)
1c00acf8:	43c8                	lw	a0,4(a5)
1c00acfa:	feb74be3          	blt	a4,a1,1c00acf0 <pos_alloc+0x12>
1c00acfe:	00b70f63          	beq	a4,a1,1c00ad1c <pos_alloc+0x3e>
1c00ad02:	00b78833          	add	a6,a5,a1
1c00ad06:	8f0d                	sub	a4,a4,a1
1c00ad08:	00e82023          	sw	a4,0(a6)
1c00ad0c:	00a82223          	sw	a0,4(a6)
1c00ad10:	ca91                	beqz	a3,1c00ad24 <pos_alloc+0x46>
1c00ad12:	0106a223          	sw	a6,4(a3)
1c00ad16:	853e                	mv	a0,a5
1c00ad18:	8082                	ret
1c00ad1a:	8082                	ret
1c00ad1c:	ca81                	beqz	a3,1c00ad2c <pos_alloc+0x4e>
1c00ad1e:	c2c8                	sw	a0,4(a3)
1c00ad20:	853e                	mv	a0,a5
1c00ad22:	8082                	ret
1c00ad24:	01062023          	sw	a6,0(a2)
1c00ad28:	853e                	mv	a0,a5
1c00ad2a:	8082                	ret
1c00ad2c:	c208                	sw	a0,0(a2)
1c00ad2e:	853e                	mv	a0,a5
1c00ad30:	8082                	ret
1c00ad32:	4501                	li	a0,0
1c00ad34:	8082                	ret

1c00ad36 <pos_allocs_init>:
1c00ad36:	1141                	addi	sp,sp,-16
1c00ad38:	1c0015b7          	lui	a1,0x1c001
1c00ad3c:	28858613          	addi	a2,a1,648 # 1c001288 <__l2_priv0_end>
1c00ad40:	1c0087b7          	lui	a5,0x1c008
1c00ad44:	c606                	sw	ra,12(sp)
1c00ad46:	c422                	sw	s0,8(sp)
1c00ad48:	28858593          	addi	a1,a1,648
1c00ad4c:	40c78633          	sub	a2,a5,a2
1c00ad50:	04f5d163          	bge	a1,a5,1c00ad92 <pos_allocs_init+0x5c>
1c00ad54:	1c001437          	lui	s0,0x1c001
1c00ad58:	26040513          	addi	a0,s0,608 # 1c001260 <pos_alloc_l2>
1c00ad5c:	3785                	jal	1c00acbc <pos_alloc_init>
1c00ad5e:	1c0105b7          	lui	a1,0x1c010
1c00ad62:	40858793          	addi	a5,a1,1032 # 1c010408 <__l2_shared_end>
1c00ad66:	1c200637          	lui	a2,0x1c200
1c00ad6a:	1c001537          	lui	a0,0x1c001
1c00ad6e:	8e1d                	sub	a2,a2,a5
1c00ad70:	40858593          	addi	a1,a1,1032
1c00ad74:	26450513          	addi	a0,a0,612 # 1c001264 <pos_alloc_l2+0x4>
1c00ad78:	3791                	jal	1c00acbc <pos_alloc_init>
1c00ad7a:	26040513          	addi	a0,s0,608
1c00ad7e:	4591                	li	a1,4
1c00ad80:	3fb9                	jal	1c00acde <pos_alloc>
1c00ad82:	40b2                	lw	ra,12(sp)
1c00ad84:	4422                	lw	s0,8(sp)
1c00ad86:	1c0017b7          	lui	a5,0x1c001
1c00ad8a:	24a7ae23          	sw	a0,604(a5) # 1c00125c <pos_alloc_l1>
1c00ad8e:	0141                	addi	sp,sp,16
1c00ad90:	8082                	ret
1c00ad92:	4581                	li	a1,0
1c00ad94:	4601                	li	a2,0
1c00ad96:	bf7d                	j	1c00ad54 <pos_allocs_init+0x1e>

1c00ad98 <pos_irq_set_handler>:
1c00ad98:	f14027f3          	csrr	a5,mhartid
1c00ad9c:	477d                	li	a4,31
1c00ad9e:	ca5797b3          	p.extractu	a5,a5,5,5
1c00ada2:	02e78d63          	beq	a5,a4,1c00addc <pos_irq_set_handler+0x44>
1c00ada6:	002007b7          	lui	a5,0x200
1c00adaa:	43b8                	lw	a4,64(a5)
1c00adac:	050a                	slli	a0,a0,0x2
1c00adae:	8d89                	sub	a1,a1,a0
1c00adb0:	8d99                	sub	a1,a1,a4
1c00adb2:	c14586b3          	p.extract	a3,a1,0,20
1c00adb6:	06f00793          	li	a5,111
1c00adba:	c1f6a7b3          	p.insert	a5,a3,0,31
1c00adbe:	d21586b3          	p.extract	a3,a1,9,1
1c00adc2:	d356a7b3          	p.insert	a5,a3,9,21
1c00adc6:	c0b586b3          	p.extract	a3,a1,0,11
1c00adca:	c146a7b3          	p.insert	a5,a3,0,20
1c00adce:	cec585b3          	p.extract	a1,a1,7,12
1c00add2:	953a                	add	a0,a0,a4
1c00add4:	cec5a7b3          	p.insert	a5,a1,7,12
1c00add8:	c11c                	sw	a5,0(a0)
1c00adda:	8082                	ret
1c00addc:	30502773          	csrr	a4,mtvec
1c00ade0:	c0073733          	p.bclr	a4,a4,0,0
1c00ade4:	b7e1                	j	1c00adac <pos_irq_set_handler+0x14>

1c00ade6 <pos_irq_init>:
1c00ade6:	f1402773          	csrr	a4,mhartid
1c00adea:	1a10a7b7          	lui	a5,0x1a10a
1c00adee:	56fd                	li	a3,-1
1c00adf0:	80d7a423          	sw	a3,-2040(a5) # 1a109808 <__l1_end+0xa1097e8>
1c00adf4:	ca571733          	p.extractu	a4,a4,5,5
1c00adf8:	1c0087b7          	lui	a5,0x1c008
1c00adfc:	46fd                	li	a3,31
1c00adfe:	00078793          	mv	a5,a5
1c00ae02:	00d70663          	beq	a4,a3,1c00ae0e <pos_irq_init+0x28>
1c00ae06:	00200737          	lui	a4,0x200
1c00ae0a:	c33c                	sw	a5,64(a4)
1c00ae0c:	8082                	ret
1c00ae0e:	c007c7b3          	p.bset	a5,a5,0,0
1c00ae12:	30579073          	csrw	mtvec,a5
1c00ae16:	8082                	ret

1c00ae18 <pos_soc_event_init>:
1c00ae18:	1141                	addi	sp,sp,-16
1c00ae1a:	c606                	sw	ra,12(sp)
1c00ae1c:	1a106737          	lui	a4,0x1a106
1c00ae20:	57fd                	li	a5,-1
1c00ae22:	c35c                	sw	a5,4(a4)
1c00ae24:	c71c                	sw	a5,8(a4)
1c00ae26:	c75c                	sw	a5,12(a4)
1c00ae28:	cb1c                	sw	a5,16(a4)
1c00ae2a:	cb5c                	sw	a5,20(a4)
1c00ae2c:	cf1c                	sw	a5,24(a4)
1c00ae2e:	cf5c                	sw	a5,28(a4)
1c00ae30:	1c00b5b7          	lui	a1,0x1c00b
1c00ae34:	02070693          	addi	a3,a4,32 # 1a106020 <__l1_end+0xa106000>
1c00ae38:	c29c                	sw	a5,0(a3)
1c00ae3a:	45a58593          	addi	a1,a1,1114 # 1c00b45a <pos_soc_event_handler_asm>
1c00ae3e:	4569                	li	a0,26
1c00ae40:	3fa1                	jal	1c00ad98 <pos_irq_set_handler>
1c00ae42:	40b2                	lw	ra,12(sp)
1c00ae44:	1a10a7b7          	lui	a5,0x1a10a
1c00ae48:	04000737          	lui	a4,0x4000
1c00ae4c:	80e7a223          	sw	a4,-2044(a5) # 1a109804 <__l1_end+0xa1097e4>
1c00ae50:	0141                	addi	sp,sp,16
1c00ae52:	8082                	ret

1c00ae54 <pos_time_poweroff>:
1c00ae54:	1a10b7b7          	lui	a5,0x1a10b
1c00ae58:	0791                	addi	a5,a5,4
1c00ae5a:	0087a783          	lw	a5,8(a5) # 1a10b008 <__l1_end+0xa10afe8>
1c00ae5e:	1c001737          	lui	a4,0x1c001
1c00ae62:	22f72423          	sw	a5,552(a4) # 1c001228 <pos_time_timer_count>
1c00ae66:	4501                	li	a0,0
1c00ae68:	8082                	ret

1c00ae6a <pos_time_poweron>:
1c00ae6a:	1c0017b7          	lui	a5,0x1c001
1c00ae6e:	2287a703          	lw	a4,552(a5) # 1c001228 <pos_time_timer_count>
1c00ae72:	1a10b7b7          	lui	a5,0x1a10b
1c00ae76:	0791                	addi	a5,a5,4
1c00ae78:	00e7a423          	sw	a4,8(a5) # 1a10b008 <__l1_end+0xa10afe8>
1c00ae7c:	4501                	li	a0,0
1c00ae7e:	8082                	ret

1c00ae80 <pos_time_timer_handler>:
1c00ae80:	1c0018b7          	lui	a7,0x1c001
1c00ae84:	22888893          	addi	a7,a7,552 # 1c001228 <pos_time_timer_count>
1c00ae88:	0048a783          	lw	a5,4(a7)
1c00ae8c:	1a10b637          	lui	a2,0x1a10b
1c00ae90:	0611                	addi	a2,a2,4
1c00ae92:	00862603          	lw	a2,8(a2) # 1a10b008 <__l1_end+0xa10afe8>
1c00ae96:	cfb5                	beqz	a5,1c00af12 <pos_time_timer_handler+0x92>
1c00ae98:	80000537          	lui	a0,0x80000
1c00ae9c:	01c02583          	lw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00aea0:	02002683          	lw	a3,32(zero) # 20 <pos_sched_last>
1c00aea4:	4801                	li	a6,0
1c00aea6:	4301                	li	t1,0
1c00aea8:	ffe54513          	xori	a0,a0,-2
1c00aeac:	a031                	j	1c00aeb8 <pos_time_timer_handler+0x38>
1c00aeae:	c29c                	sw	a5,0(a3)
1c00aeb0:	cb39                	beqz	a4,1c00af06 <pos_time_timer_handler+0x86>
1c00aeb2:	86be                	mv	a3,a5
1c00aeb4:	4805                	li	a6,1
1c00aeb6:	87ba                	mv	a5,a4
1c00aeb8:	5fd8                	lw	a4,60(a5)
1c00aeba:	40e60733          	sub	a4,a2,a4
1c00aebe:	00e56a63          	bltu	a0,a4,1c00aed2 <pos_time_timer_handler+0x52>
1c00aec2:	4398                	lw	a4,0(a5)
1c00aec4:	0007a023          	sw	zero,0(a5)
1c00aec8:	f1fd                	bnez	a1,1c00aeae <pos_time_timer_handler+0x2e>
1c00aeca:	85be                	mv	a1,a5
1c00aecc:	cf1d                	beqz	a4,1c00af0a <pos_time_timer_handler+0x8a>
1c00aece:	4305                	li	t1,1
1c00aed0:	b7cd                	j	1c00aeb2 <pos_time_timer_handler+0x32>
1c00aed2:	00030463          	beqz	t1,1c00aeda <pos_time_timer_handler+0x5a>
1c00aed6:	00b02e23          	sw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00aeda:	00080463          	beqz	a6,1c00aee2 <pos_time_timer_handler+0x62>
1c00aede:	02d02023          	sw	a3,32(zero) # 20 <pos_sched_last>
1c00aee2:	1a10b737          	lui	a4,0x1a10b
1c00aee6:	0711                	addi	a4,a4,4
1c00aee8:	00f8a223          	sw	a5,4(a7)
1c00aeec:	00872683          	lw	a3,8(a4) # 1a10b008 <__l1_end+0xa10afe8>
1c00aef0:	5fdc                	lw	a5,60(a5)
1c00aef2:	40c78633          	sub	a2,a5,a2
1c00aef6:	9636                	add	a2,a2,a3
1c00aef8:	00c72823          	sw	a2,16(a4)
1c00aefc:	08500793          	li	a5,133
1c00af00:	00f72023          	sw	a5,0(a4)
1c00af04:	8082                	ret
1c00af06:	00030463          	beqz	t1,1c00af0e <pos_time_timer_handler+0x8e>
1c00af0a:	00b02e23          	sw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00af0e:	02f02023          	sw	a5,32(zero) # 20 <pos_sched_last>
1c00af12:	1a10b7b7          	lui	a5,0x1a10b
1c00af16:	08100713          	li	a4,129
1c00af1a:	0791                	addi	a5,a5,4
1c00af1c:	0008a223          	sw	zero,4(a7)
1c00af20:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c00af24:	f14027f3          	csrr	a5,mhartid
1c00af28:	477d                	li	a4,31
1c00af2a:	ca5797b3          	p.extractu	a5,a5,5,5
1c00af2e:	00e78a63          	beq	a5,a4,1c00af42 <pos_time_timer_handler+0xc2>
1c00af32:	6785                	lui	a5,0x1
1c00af34:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00af38:	00204737          	lui	a4,0x204
1c00af3c:	02f72423          	sw	a5,40(a4) # 204028 <__l1_heap_size+0x1c4048>
1c00af40:	8082                	ret
1c00af42:	6785                	lui	a5,0x1
1c00af44:	1a10a737          	lui	a4,0x1a10a
1c00af48:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00af4c:	80f72a23          	sw	a5,-2028(a4) # 1a109814 <__l1_end+0xa1097f4>
1c00af50:	8082                	ret

1c00af52 <pos_time_init>:
1c00af52:	1141                	addi	sp,sp,-16
1c00af54:	c422                	sw	s0,8(sp)
1c00af56:	1a10b7b7          	lui	a5,0x1a10b
1c00af5a:	1c001437          	lui	s0,0x1c001
1c00af5e:	22840413          	addi	s0,s0,552 # 1c001228 <pos_time_timer_count>
1c00af62:	08300713          	li	a4,131
1c00af66:	0791                	addi	a5,a5,4
1c00af68:	c606                	sw	ra,12(sp)
1c00af6a:	00042223          	sw	zero,4(s0)
1c00af6e:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c00af72:	1c00b5b7          	lui	a1,0x1c00b
1c00af76:	43658593          	addi	a1,a1,1078 # 1c00b436 <pos_time_timer_handler_asm>
1c00af7a:	452d                	li	a0,11
1c00af7c:	3d31                	jal	1c00ad98 <pos_irq_set_handler>
1c00af7e:	6785                	lui	a5,0x1
1c00af80:	1a10a737          	lui	a4,0x1a10a
1c00af84:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00af88:	1c00b637          	lui	a2,0x1c00b
1c00af8c:	00840513          	addi	a0,s0,8
1c00af90:	4681                	li	a3,0
1c00af92:	e5460613          	addi	a2,a2,-428 # 1c00ae54 <pos_time_poweroff>
1c00af96:	4589                	li	a1,2
1c00af98:	80f72223          	sw	a5,-2044(a4) # 1a109804 <__l1_end+0xa1097e4>
1c00af9c:	3945                	jal	1c00ac4c <pos_cbsys_add>
1c00af9e:	01840513          	addi	a0,s0,24
1c00afa2:	4422                	lw	s0,8(sp)
1c00afa4:	40b2                	lw	ra,12(sp)
1c00afa6:	1c00b637          	lui	a2,0x1c00b
1c00afaa:	4681                	li	a3,0
1c00afac:	e6a60613          	addi	a2,a2,-406 # 1c00ae6a <pos_time_poweron>
1c00afb0:	458d                	li	a1,3
1c00afb2:	0141                	addi	sp,sp,16
1c00afb4:	b961                	j	1c00ac4c <pos_cbsys_add>

1c00afb6 <pi_uart_conf_init>:
1c00afb6:	6771                	lui	a4,0x1c
1c00afb8:	4785                	li	a5,1
1c00afba:	20070713          	addi	a4,a4,512 # 1c200 <pos_soc_event_callback+0x1bdbc>
1c00afbe:	c118                	sw	a4,0(a0)
1c00afc0:	00f51223          	sh	a5,4(a0) # 80000004 <__l2_shared_end+0x63fefbfc>
1c00afc4:	00f503a3          	sb	a5,7(a0)
1c00afc8:	00f51423          	sh	a5,8(a0)
1c00afcc:	8082                	ret

1c00afce <pi_uart_open>:
1c00afce:	7179                	addi	sp,sp,-48
1c00afd0:	d606                	sw	ra,44(sp)
1c00afd2:	d422                	sw	s0,40(sp)
1c00afd4:	d226                	sw	s1,36(sp)
1c00afd6:	d04a                	sw	s2,32(sp)
1c00afd8:	ce4e                	sw	s3,28(sp)
1c00afda:	cc52                	sw	s4,24(sp)
1c00afdc:	ca56                	sw	s5,20(sp)
1c00afde:	c85a                	sw	s6,16(sp)
1c00afe0:	c65e                	sw	s7,12(sp)
1c00afe2:	c462                	sw	s8,8(sp)
1c00afe4:	c266                	sw	s9,4(sp)
1c00afe6:	300479f3          	csrrci	s3,mstatus,8
1c00afea:	4158                	lw	a4,4(a0)
1c00afec:	1c0107b7          	lui	a5,0x1c010
1c00aff0:	3b478913          	addi	s2,a5,948 # 1c0103b4 <pos_uart>
1c00aff4:	00974483          	lbu	s1,9(a4)
1c00aff8:	00072c03          	lw	s8,0(a4)
1c00affc:	00349a13          	slli	s4,s1,0x3
1c00b000:	409a0bb3          	sub	s7,s4,s1
1c00b004:	0b8e                	slli	s7,s7,0x3
1c00b006:	01790ab3          	add	s5,s2,s7
1c00b00a:	000aa703          	lw	a4,0(s5)
1c00b00e:	00249413          	slli	s0,s1,0x2
1c00b012:	01552423          	sw	s5,8(a0)
1c00b016:	00140b13          	addi	s6,s0,1
1c00b01a:	cb55                	beqz	a4,1c00b0ce <pi_uart_open+0x100>
1c00b01c:	409a07b3          	sub	a5,s4,s1
1c00b020:	078e                	slli	a5,a5,0x3
1c00b022:	97ca                	add	a5,a5,s2
1c00b024:	0705                	addi	a4,a4,1
1c00b026:	c398                	sw	a4,0(a5)
1c00b028:	c3c0                	sw	s0,4(a5)
1c00b02a:	0187a423          	sw	s8,8(a5)
1c00b02e:	1a102737          	lui	a4,0x1a102
1c00b032:	4314                	lw	a3,0(a4)
1c00b034:	4605                	li	a2,1
1c00b036:	1a1065b7          	lui	a1,0x1a106
1c00b03a:	009614b3          	sll	s1,a2,s1
1c00b03e:	40545793          	srai	a5,s0,0x5
1c00b042:	00458813          	addi	a6,a1,4 # 1a106004 <__l1_end+0xa105fe4>
1c00b046:	8cd5                	or	s1,s1,a3
1c00b048:	078a                	slli	a5,a5,0x2
1c00b04a:	0a97222b          	p.sw	s1,164(a4!) # 1a1020a4 <__l1_end+0xa102084>
1c00b04e:	010788b3          	add	a7,a5,a6
1c00b052:	0008a503          	lw	a0,0(a7)
1c00b056:	f4543433          	p.bclr	s0,s0,26,5
1c00b05a:	00861433          	sll	s0,a2,s0
1c00b05e:	fff44413          	not	s0,s0
1c00b062:	405b5693          	srai	a3,s6,0x5
1c00b066:	8d61                	and	a0,a0,s0
1c00b068:	068a                	slli	a3,a3,0x2
1c00b06a:	00a8a023          	sw	a0,0(a7)
1c00b06e:	96c2                	add	a3,a3,a6
1c00b070:	4288                	lw	a0,0(a3)
1c00b072:	01661633          	sll	a2,a2,s6
1c00b076:	fff64613          	not	a2,a2
1c00b07a:	8e69                	and	a2,a2,a0
1c00b07c:	04458593          	addi	a1,a1,68
1c00b080:	c290                	sw	a2,0(a3)
1c00b082:	97ae                	add	a5,a5,a1
1c00b084:	4394                	lw	a3,0(a5)
1c00b086:	8c75                	and	s0,s0,a3
1c00b088:	c380                	sw	s0,0(a5)
1c00b08a:	008aa603          	lw	a2,8(s5)
1c00b08e:	00802583          	lw	a1,8(zero) # 8 <pos_cluster_pool>
1c00b092:	00165793          	srli	a5,a2,0x1
1c00b096:	004aa683          	lw	a3,4(s5)
1c00b09a:	97ae                	add	a5,a5,a1
1c00b09c:	02c7d7b3          	divu	a5,a5,a2
1c00b0a0:	069e                	slli	a3,a3,0x7
1c00b0a2:	9736                	add	a4,a4,a3
1c00b0a4:	17fd                	addi	a5,a5,-1
1c00b0a6:	07c2                	slli	a5,a5,0x10
1c00b0a8:	3067e793          	ori	a5,a5,774
1c00b0ac:	c31c                	sw	a5,0(a4)
1c00b0ae:	30099073          	csrw	mstatus,s3
1c00b0b2:	50b2                	lw	ra,44(sp)
1c00b0b4:	5422                	lw	s0,40(sp)
1c00b0b6:	5492                	lw	s1,36(sp)
1c00b0b8:	5902                	lw	s2,32(sp)
1c00b0ba:	49f2                	lw	s3,28(sp)
1c00b0bc:	4a62                	lw	s4,24(sp)
1c00b0be:	4ad2                	lw	s5,20(sp)
1c00b0c0:	4b42                	lw	s6,16(sp)
1c00b0c2:	4bb2                	lw	s7,12(sp)
1c00b0c4:	4c22                	lw	s8,8(sp)
1c00b0c6:	4c92                	lw	s9,4(sp)
1c00b0c8:	4501                	li	a0,0
1c00b0ca:	6145                	addi	sp,sp,48
1c00b0cc:	8082                	ret
1c00b0ce:	00149c93          	slli	s9,s1,0x1
1c00b0d2:	024b8513          	addi	a0,s7,36
1c00b0d6:	8622                	mv	a2,s0
1c00b0d8:	85e6                	mv	a1,s9
1c00b0da:	954a                	add	a0,a0,s2
1c00b0dc:	22b5                	jal	1c00b248 <pos_udma_create_channel>
1c00b0de:	010b8513          	addi	a0,s7,16
1c00b0e2:	865a                	mv	a2,s6
1c00b0e4:	001c8593          	addi	a1,s9,1
1c00b0e8:	954a                	add	a0,a0,s2
1c00b0ea:	2ab9                	jal	1c00b248 <pos_udma_create_channel>
1c00b0ec:	000aa703          	lw	a4,0(s5)
1c00b0f0:	b735                	j	1c00b01c <pi_uart_open+0x4e>

1c00b0f2 <pi_uart_close>:
1c00b0f2:	30047873          	csrrci	a6,mstatus,8
1c00b0f6:	4518                	lw	a4,8(a0)
1c00b0f8:	1a1026b7          	lui	a3,0x1a102
1c00b0fc:	09868693          	addi	a3,a3,152 # 1a102098 <__l1_end+0xa102078>
1c00b100:	4310                	lw	a2,0(a4)
1c00b102:	435c                	lw	a5,4(a4)
1c00b104:	167d                	addi	a2,a2,-1
1c00b106:	079e                	slli	a5,a5,0x7
1c00b108:	c310                	sw	a2,0(a4)
1c00b10a:	00d78633          	add	a2,a5,a3
1c00b10e:	4210                	lw	a2,0(a2)
1c00b110:	8a41                	andi	a2,a2,16
1c00b112:	ca11                	beqz	a2,1c00b126 <pi_uart_close+0x34>
1c00b114:	10500073          	wfi
1c00b118:	435c                	lw	a5,4(a4)
1c00b11a:	079e                	slli	a5,a5,0x7
1c00b11c:	00d78633          	add	a2,a5,a3
1c00b120:	4210                	lw	a2,0(a2)
1c00b122:	8a41                	andi	a2,a2,16
1c00b124:	fa65                	bnez	a2,1c00b114 <pi_uart_close+0x22>
1c00b126:	1a1026b7          	lui	a3,0x1a102
1c00b12a:	0a068693          	addi	a3,a3,160 # 1a1020a0 <__l1_end+0xa102080>
1c00b12e:	97b6                	add	a5,a5,a3
1c00b130:	4394                	lw	a3,0(a5)
1c00b132:	fc16b6b3          	p.bclr	a3,a3,30,1
1c00b136:	feed                	bnez	a3,1c00b130 <pi_uart_close+0x3e>
1c00b138:	f14025f3          	csrr	a1,mhartid
1c00b13c:	1a10a637          	lui	a2,0x1a10a
1c00b140:	80460893          	addi	a7,a2,-2044 # 1a109804 <__l1_end+0xa1097e4>
1c00b144:	80860513          	addi	a0,a2,-2040
1c00b148:	03200693          	li	a3,50
1c00b14c:	ca5595b3          	p.extractu	a1,a1,5,5
1c00b150:	4e7d                	li	t3,31
1c00b152:	6791                	lui	a5,0x4
1c00b154:	00204337          	lui	t1,0x204
1c00b158:	81460613          	addi	a2,a2,-2028
1c00b15c:	05c58263          	beq	a1,t3,1c00b1a0 <pi_uart_close+0xae>
1c00b160:	02f32423          	sw	a5,40(t1) # 204028 <__l1_heap_size+0x1c4048>
1c00b164:	00f8a023          	sw	a5,0(a7)
1c00b168:	10500073          	wfi
1c00b16c:	c11c                	sw	a5,0(a0)
1c00b16e:	16fd                	addi	a3,a3,-1
1c00b170:	f6f5                	bnez	a3,1c00b15c <pi_uart_close+0x6a>
1c00b172:	435c                	lw	a5,4(a4)
1c00b174:	1a1026b7          	lui	a3,0x1a102
1c00b178:	0a468613          	addi	a2,a3,164 # 1a1020a4 <__l1_end+0xa102084>
1c00b17c:	079e                	slli	a5,a5,0x7
1c00b17e:	97b2                	add	a5,a5,a2
1c00b180:	00500637          	lui	a2,0x500
1c00b184:	0619                	addi	a2,a2,6
1c00b186:	c390                	sw	a2,0(a5)
1c00b188:	4350                	lw	a2,4(a4)
1c00b18a:	4785                	li	a5,1
1c00b18c:	4298                	lw	a4,0(a3)
1c00b18e:	00c797b3          	sll	a5,a5,a2
1c00b192:	fff7c793          	not	a5,a5
1c00b196:	8ff9                	and	a5,a5,a4
1c00b198:	c29c                	sw	a5,0(a3)
1c00b19a:	30081073          	csrw	mstatus,a6
1c00b19e:	8082                	ret
1c00b1a0:	c21c                	sw	a5,0(a2)
1c00b1a2:	b7c9                	j	1c00b164 <pi_uart_close+0x72>

1c00b1a4 <pi_uart_write>:
1c00b1a4:	4508                	lw	a0,8(a0)
1c00b1a6:	715d                	addi	sp,sp,-80
1c00b1a8:	1c00b7b7          	lui	a5,0x1c00b
1c00b1ac:	86b2                	mv	a3,a2
1c00b1ae:	c6a78793          	addi	a5,a5,-918 # 1c00ac6a <pos_task_handle_blocking>
1c00b1b2:	862e                	mv	a2,a1
1c00b1b4:	4701                	li	a4,0
1c00b1b6:	858a                	mv	a1,sp
1c00b1b8:	0541                	addi	a0,a0,16
1c00b1ba:	c686                	sw	ra,76(sp)
1c00b1bc:	c4a2                	sw	s0,72(sp)
1c00b1be:	c23e                	sw	a5,4(sp)
1c00b1c0:	c40a                	sw	sp,8(sp)
1c00b1c2:	00010a23          	sb	zero,20(sp)
1c00b1c6:	20c9                	jal	1c00b288 <pos_udma_enqueue>
1c00b1c8:	30047473          	csrrci	s0,mstatus,8
1c00b1cc:	01410783          	lb	a5,20(sp)
1c00b1d0:	e789                	bnez	a5,1c00b1da <pi_uart_write+0x36>
1c00b1d2:	3445                	jal	1c00ac72 <pos_task_handle>
1c00b1d4:	01410783          	lb	a5,20(sp)
1c00b1d8:	dfed                	beqz	a5,1c00b1d2 <pi_uart_write+0x2e>
1c00b1da:	30041073          	csrw	mstatus,s0
1c00b1de:	40b6                	lw	ra,76(sp)
1c00b1e0:	4426                	lw	s0,72(sp)
1c00b1e2:	4501                	li	a0,0
1c00b1e4:	6161                	addi	sp,sp,80
1c00b1e6:	8082                	ret

1c00b1e8 <pos_uart_init>:
1c00b1e8:	1c0107b7          	lui	a5,0x1c010
1c00b1ec:	3a07aa23          	sw	zero,948(a5) # 1c0103b4 <pos_uart>
1c00b1f0:	8082                	ret

1c00b1f2 <pos_udma_handle_copy>:
1c00b1f2:	41d4                	lw	a3,4(a1)
1c00b1f4:	459c                	lw	a5,8(a1)
1c00b1f6:	4198                	lw	a4,0(a1)
1c00b1f8:	c194                	sw	a3,0(a1)
1c00b1fa:	cb9d                	beqz	a5,1c00b230 <pos_udma_handle_copy+0x3e>
1c00b1fc:	0007a803          	lw	a6,0(a5)
1c00b200:	4994                	lw	a3,16(a1)
1c00b202:	4fc8                	lw	a0,28(a5)
1c00b204:	5390                	lw	a2,32(a5)
1c00b206:	c1dc                	sw	a5,4(a1)
1c00b208:	0105a423          	sw	a6,8(a1)
1c00b20c:	87b6                	mv	a5,a3
1c00b20e:	00a7a22b          	p.sw	a0,4(a5!)
1c00b212:	c390                	sw	a2,0(a5)
1c00b214:	06a1                	addi	a3,a3,8
1c00b216:	47c1                	li	a5,16
1c00b218:	c29c                	sw	a5,0(a3)
1c00b21a:	01c02683          	lw	a3,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b21e:	00072023          	sw	zero,0(a4)
1c00b222:	ce91                	beqz	a3,1c00b23e <pos_udma_handle_copy+0x4c>
1c00b224:	02002683          	lw	a3,32(zero) # 20 <pos_sched_last>
1c00b228:	c298                	sw	a4,0(a3)
1c00b22a:	02e02023          	sw	a4,32(zero) # 20 <pos_sched_last>
1c00b22e:	8082                	ret
1c00b230:	01c02683          	lw	a3,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b234:	0005a223          	sw	zero,4(a1)
1c00b238:	00072023          	sw	zero,0(a4)
1c00b23c:	f6e5                	bnez	a3,1c00b224 <pos_udma_handle_copy+0x32>
1c00b23e:	00e02e23          	sw	a4,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b242:	02e02023          	sw	a4,32(zero) # 20 <pos_sched_last>
1c00b246:	8082                	ret

1c00b248 <pos_udma_create_channel>:
1c00b248:	060a                	slli	a2,a2,0x2
1c00b24a:	44400693          	li	a3,1092
1c00b24e:	04400713          	li	a4,68
1c00b252:	4015d793          	srai	a5,a1,0x1
1c00b256:	96b2                	add	a3,a3,a2
1c00b258:	9732                	add	a4,a4,a2
1c00b25a:	1a102637          	lui	a2,0x1a102
1c00b25e:	08060613          	addi	a2,a2,128 # 1a102080 <__l1_end+0xa102060>
1c00b262:	079e                	slli	a5,a5,0x7
1c00b264:	97b2                	add	a5,a5,a2
1c00b266:	0592                	slli	a1,a1,0x4
1c00b268:	1c00b637          	lui	a2,0x1c00b
1c00b26c:	89c1                	andi	a1,a1,16
1c00b26e:	1f260613          	addi	a2,a2,498 # 1c00b1f2 <pos_udma_handle_copy>
1c00b272:	c290                	sw	a2,0(a3)
1c00b274:	c308                	sw	a0,0(a4)
1c00b276:	95be                	add	a1,a1,a5
1c00b278:	00052023          	sw	zero,0(a0)
1c00b27c:	00052223          	sw	zero,4(a0)
1c00b280:	00052423          	sw	zero,8(a0)
1c00b284:	c90c                	sw	a1,16(a0)
1c00b286:	8082                	ret

1c00b288 <pos_udma_enqueue>:
1c00b288:	411c                	lw	a5,0(a0)
1c00b28a:	cb8d                	beqz	a5,1c00b2bc <pos_udma_enqueue+0x34>
1c00b28c:	415c                	lw	a5,4(a0)
1c00b28e:	cf81                	beqz	a5,1c00b2a6 <pos_udma_enqueue+0x1e>
1c00b290:	451c                	lw	a5,8(a0)
1c00b292:	cdd0                	sw	a2,28(a1)
1c00b294:	d194                	sw	a3,32(a1)
1c00b296:	d1d8                	sw	a4,36(a1)
1c00b298:	cf8d                	beqz	a5,1c00b2d2 <pos_udma_enqueue+0x4a>
1c00b29a:	455c                	lw	a5,12(a0)
1c00b29c:	c38c                	sw	a1,0(a5)
1c00b29e:	c54c                	sw	a1,12(a0)
1c00b2a0:	0005a023          	sw	zero,0(a1)
1c00b2a4:	8082                	ret
1c00b2a6:	491c                	lw	a5,16(a0)
1c00b2a8:	c14c                	sw	a1,4(a0)
1c00b2aa:	c0474733          	p.bset	a4,a4,0,4
1c00b2ae:	85be                	mv	a1,a5
1c00b2b0:	00c5a22b          	p.sw	a2,4(a1!)
1c00b2b4:	c194                	sw	a3,0(a1)
1c00b2b6:	07a1                	addi	a5,a5,8
1c00b2b8:	c398                	sw	a4,0(a5)
1c00b2ba:	8082                	ret
1c00b2bc:	491c                	lw	a5,16(a0)
1c00b2be:	c10c                	sw	a1,0(a0)
1c00b2c0:	c0474733          	p.bset	a4,a4,0,4
1c00b2c4:	85be                	mv	a1,a5
1c00b2c6:	00c5a22b          	p.sw	a2,4(a1!)
1c00b2ca:	c194                	sw	a3,0(a1)
1c00b2cc:	07a1                	addi	a5,a5,8
1c00b2ce:	c398                	sw	a4,0(a5)
1c00b2d0:	8082                	ret
1c00b2d2:	c50c                	sw	a1,8(a0)
1c00b2d4:	c54c                	sw	a1,12(a0)
1c00b2d6:	0005a023          	sw	zero,0(a1)
1c00b2da:	8082                	ret

1c00b2dc <pos_cluster_push_fc_event>:
1c00b2dc:	00204637          	lui	a2,0x204
1c00b2e0:	0c060613          	addi	a2,a2,192 # 2040c0 <__l1_heap_size+0x1c40e0>
1c00b2e4:	421c                	lw	a5,0(a2)
1c00b2e6:	f1402373          	csrr	t1,mhartid
1c00b2ea:	1c0018b7          	lui	a7,0x1c001
1c00b2ee:	26888893          	addi	a7,a7,616 # 1c001268 <pos_cluster>
1c00b2f2:	7e037313          	andi	t1,t1,2016
1c00b2f6:	006885b3          	add	a1,a7,t1
1c00b2fa:	49dc                	lw	a5,20(a1)
1c00b2fc:	c38d                	beqz	a5,1c00b31e <pos_cluster_push_fc_event+0x42>
1c00b2fe:	002047b7          	lui	a5,0x204
1c00b302:	4709                	li	a4,2
1c00b304:	03c78813          	addi	a6,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c00b308:	0c07a023          	sw	zero,192(a5)
1c00b30c:	00e7a423          	sw	a4,8(a5)
1c00b310:	00082683          	lw	a3,0(a6)
1c00b314:	00e7a223          	sw	a4,4(a5)
1c00b318:	4214                	lw	a3,0(a2)
1c00b31a:	49d4                	lw	a3,20(a1)
1c00b31c:	f6f5                	bnez	a3,1c00b308 <pos_cluster_push_fc_event+0x2c>
1c00b31e:	989a                	add	a7,a7,t1
1c00b320:	1a10a7b7          	lui	a5,0x1a10a
1c00b324:	00a8aa23          	sw	a0,20(a7)
1c00b328:	4709                	li	a4,2
1c00b32a:	80e7a823          	sw	a4,-2032(a5) # 1a109810 <__l1_end+0xa1097f0>
1c00b32e:	002047b7          	lui	a5,0x204
1c00b332:	0c07a023          	sw	zero,192(a5) # 2040c0 <__l1_heap_size+0x1c40e0>
1c00b336:	8082                	ret

1c00b338 <pos_init_entry>:
1c00b338:	7a101073          	csrw	tdata1,zero
1c00b33c:	f1402573          	csrr	a0,mhartid
1c00b340:	01f57593          	andi	a1,a0,31
1c00b344:	8115                	srli	a0,a0,0x5
1c00b346:	467d                	li	a2,31
1c00b348:	00c50463          	beq	a0,a2,1c00b350 <pos_init_entry+0x18>
1c00b34c:	4b50406f          	j	1c010000 <__cluster_text_start>
1c00b350:	ffff6297          	auipc	t0,0xffff6
1c00b354:	f0028293          	addi	t0,t0,-256 # 1c001250 <pos_io_uart_enabled>
1c00b358:	ffff6317          	auipc	t1,0xffff6
1c00b35c:	f3030313          	addi	t1,t1,-208 # 1c001288 <__l2_priv0_end>
1c00b360:	0002a023          	sw	zero,0(t0)
1c00b364:	0291                	addi	t0,t0,4
1c00b366:	fe62ede3          	bltu	t0,t1,1c00b360 <pos_init_entry+0x28>
1c00b36a:	ffff6117          	auipc	sp,0xffff6
1c00b36e:	ea610113          	addi	sp,sp,-346 # 1c001210 <pos_cbsys_first>
1c00b372:	00000297          	auipc	t0,0x0
1c00b376:	87a28293          	addi	t0,t0,-1926 # 1c00abec <pos_init_start>
1c00b37a:	000280e7          	jalr	t0
1c00b37e:	00000513          	li	a0,0
1c00b382:	00000593          	li	a1,0
1c00b386:	ffffd397          	auipc	t2,0xffffd
1c00b38a:	55a38393          	addi	t2,t2,1370 # 1c0088e0 <main>
1c00b38e:	000380e7          	jalr	t2
1c00b392:	842a                	mv	s0,a0
1c00b394:	8522                	mv	a0,s0
1c00b396:	ffffd297          	auipc	t0,0xffffd
1c00b39a:	78028293          	addi	t0,t0,1920 # 1c008b16 <exit>
1c00b39e:	000280e7          	jalr	t0

1c00b3a2 <pos_irq_call_external_c_function>:
1c00b3a2:	7119                	addi	sp,sp,-128
1c00b3a4:	c006                	sw	ra,0(sp)
1c00b3a6:	c20e                	sw	gp,4(sp)
1c00b3a8:	c412                	sw	tp,8(sp)
1c00b3aa:	c616                	sw	t0,12(sp)
1c00b3ac:	c81a                	sw	t1,16(sp)
1c00b3ae:	ca1e                	sw	t2,20(sp)
1c00b3b0:	d236                	sw	a3,36(sp)
1c00b3b2:	d43a                	sw	a4,40(sp)
1c00b3b4:	d63e                	sw	a5,44(sp)
1c00b3b6:	d842                	sw	a6,48(sp)
1c00b3b8:	da46                	sw	a7,52(sp)
1c00b3ba:	dc72                	sw	t3,56(sp)
1c00b3bc:	de76                	sw	t4,60(sp)
1c00b3be:	c0fa                	sw	t5,64(sp)
1c00b3c0:	c6fe                	sw	t6,76(sp)
1c00b3c2:	000600e7          	jalr	a2
1c00b3c6:	4082                	lw	ra,0(sp)
1c00b3c8:	4192                	lw	gp,4(sp)
1c00b3ca:	4222                	lw	tp,8(sp)
1c00b3cc:	42b2                	lw	t0,12(sp)
1c00b3ce:	4342                	lw	t1,16(sp)
1c00b3d0:	43d2                	lw	t2,20(sp)
1c00b3d2:	5692                	lw	a3,36(sp)
1c00b3d4:	5722                	lw	a4,40(sp)
1c00b3d6:	57b2                	lw	a5,44(sp)
1c00b3d8:	5842                	lw	a6,48(sp)
1c00b3da:	58d2                	lw	a7,52(sp)
1c00b3dc:	5e62                	lw	t3,56(sp)
1c00b3de:	5ef2                	lw	t4,60(sp)
1c00b3e0:	4f06                	lw	t5,64(sp)
1c00b3e2:	4fb6                	lw	t6,76(sp)
1c00b3e4:	6109                	addi	sp,sp,128
1c00b3e6:	8482                	jr	s1

1c00b3e8 <pos_irq_call_external_c_function_full>:
1c00b3e8:	7119                	addi	sp,sp,-128
1c00b3ea:	c006                	sw	ra,0(sp)
1c00b3ec:	c20e                	sw	gp,4(sp)
1c00b3ee:	c412                	sw	tp,8(sp)
1c00b3f0:	c616                	sw	t0,12(sp)
1c00b3f2:	c81a                	sw	t1,16(sp)
1c00b3f4:	ca1e                	sw	t2,20(sp)
1c00b3f6:	cc2a                	sw	a0,24(sp)
1c00b3f8:	ce2e                	sw	a1,28(sp)
1c00b3fa:	d236                	sw	a3,36(sp)
1c00b3fc:	d43a                	sw	a4,40(sp)
1c00b3fe:	d63e                	sw	a5,44(sp)
1c00b400:	d842                	sw	a6,48(sp)
1c00b402:	da46                	sw	a7,52(sp)
1c00b404:	dc72                	sw	t3,56(sp)
1c00b406:	de76                	sw	t4,60(sp)
1c00b408:	c0fa                	sw	t5,64(sp)
1c00b40a:	c6fe                	sw	t6,76(sp)
1c00b40c:	000600e7          	jalr	a2
1c00b410:	4082                	lw	ra,0(sp)
1c00b412:	4192                	lw	gp,4(sp)
1c00b414:	4222                	lw	tp,8(sp)
1c00b416:	42b2                	lw	t0,12(sp)
1c00b418:	4342                	lw	t1,16(sp)
1c00b41a:	43d2                	lw	t2,20(sp)
1c00b41c:	4562                	lw	a0,24(sp)
1c00b41e:	45f2                	lw	a1,28(sp)
1c00b420:	5692                	lw	a3,36(sp)
1c00b422:	5722                	lw	a4,40(sp)
1c00b424:	57b2                	lw	a5,44(sp)
1c00b426:	5842                	lw	a6,48(sp)
1c00b428:	58d2                	lw	a7,52(sp)
1c00b42a:	5e62                	lw	t3,56(sp)
1c00b42c:	5ef2                	lw	t4,60(sp)
1c00b42e:	4f06                	lw	t5,64(sp)
1c00b430:	4fb6                	lw	t6,76(sp)
1c00b432:	6109                	addi	sp,sp,128
1c00b434:	8482                	jr	s1

1c00b436 <pos_time_timer_handler_asm>:
1c00b436:	1161                	addi	sp,sp,-8
1c00b438:	c032                	sw	a2,0(sp)
1c00b43a:	c226                	sw	s1,4(sp)
1c00b43c:	00000617          	auipc	a2,0x0
1c00b440:	a4460613          	addi	a2,a2,-1468 # 1c00ae80 <pos_time_timer_handler>
1c00b444:	00000497          	auipc	s1,0x0
1c00b448:	00c48493          	addi	s1,s1,12 # 1c00b450 <pos_time_timer_handler_asm_ret>
1c00b44c:	f9dff06f          	j	1c00b3e8 <pos_irq_call_external_c_function_full>

1c00b450 <pos_time_timer_handler_asm_ret>:
1c00b450:	4492                	lw	s1,4(sp)
1c00b452:	4602                	lw	a2,0(sp)
1c00b454:	0121                	addi	sp,sp,8
1c00b456:	30200073          	mret

1c00b45a <pos_soc_event_handler_asm>:
1c00b45a:	7119                	addi	sp,sp,-128
1c00b45c:	c022                	sw	s0,0(sp)
1c00b45e:	c226                	sw	s1,4(sp)
1c00b460:	c42a                	sw	a0,8(sp)
1c00b462:	c62e                	sw	a1,12(sp)
1c00b464:	c832                	sw	a2,16(sp)
1c00b466:	1a10a437          	lui	s0,0x1a10a
1c00b46a:	80040413          	addi	s0,s0,-2048 # 1a109800 <__l1_end+0xa1097e0>
1c00b46e:	5048                	lw	a0,36(s0)
1c00b470:	00251593          	slli	a1,a0,0x2
1c00b474:	44400293          	li	t0,1092
1c00b478:	2055f603          	p.lw	a2,t0(a1)
1c00b47c:	04400293          	li	t0,68
1c00b480:	2055f583          	p.lw	a1,t0(a1)
1c00b484:	00000497          	auipc	s1,0x0
1c00b488:	02848493          	addi	s1,s1,40 # 1c00b4ac <pos_soc_event_handler_end_asm>
1c00b48c:	f17ff06f          	j	1c00b3a2 <pos_irq_call_external_c_function>

1c00b490 <pos_soc_event_no_udma_asm>:
1c00b490:	00555593          	srli	a1,a0,0x5
1c00b494:	058a                	slli	a1,a1,0x2
1c00b496:	02400293          	li	t0,36
1c00b49a:	2055f603          	p.lw	a2,t0(a1)
1c00b49e:	897d                	andi	a0,a0,31
1c00b4a0:	80a64633          	p.bsetr	a2,a2,a0
1c00b4a4:	02400293          	li	t0,36
1c00b4a8:	00c5e2a3          	p.sw	a2,t0(a1)

1c00b4ac <pos_soc_event_handler_end_asm>:
1c00b4ac:	4402                	lw	s0,0(sp)
1c00b4ae:	4492                	lw	s1,4(sp)
1c00b4b0:	4522                	lw	a0,8(sp)
1c00b4b2:	45b2                	lw	a1,12(sp)
1c00b4b4:	4642                	lw	a2,16(sp)
1c00b4b6:	6109                	addi	sp,sp,128
1c00b4b8:	30200073          	mret

Disassembly of section .l2_data:

1c010000 <__cluster_text_start>:
1c010000:	f1402573          	csrr	a0,mhartid
1c010004:	01f57593          	andi	a1,a0,31
1c010008:	8115                	srli	a0,a0,0x5
1c01000a:	000702b7          	lui	t0,0x70
1c01000e:	00204337          	lui	t1,0x204
1c010012:	00532023          	sw	t0,0(t1) # 204000 <__l1_heap_size+0x1c4020>
1c010016:	4381                	li	t2,0
1c010018:	0e759063          	bne	a1,t2,1c0100f8 <pos_slave_start>
1c01001c:	20000293          	li	t0,512
1c010020:	00204337          	lui	t1,0x204
1c010024:	00532a23          	sw	t0,20(t1) # 204014 <__l1_heap_size+0x1c4034>
1c010028:	00800413          	li	s0,8
1c01002c:	002049b7          	lui	s3,0x204
1c010030:	4a09                	li	s4,2
1c010032:	00000a97          	auipc	s5,0x0
1c010036:	038a8a93          	addi	s5,s5,56 # 1c01006a <pos_master_event>
1c01003a:	ffff1b97          	auipc	s7,0xffff1
1c01003e:	22eb8b93          	addi	s7,s7,558 # 1c001268 <pos_cluster>
1c010042:	02c00393          	li	t2,44
1c010046:	02a383b3          	mul	t2,t2,a0
1c01004a:	9b9e                	add	s7,s7,t2
1c01004c:	0bd1                	addi	s7,s7,20
1c01004e:	1a10acb7          	lui	s9,0x1a10a
1c010052:	810c8c93          	addi	s9,s9,-2032 # 1a109810 <__l1_end+0xa1097f0>
1c010056:	4c09                	li	s8,2
1c010058:	00000d17          	auipc	s10,0x0
1c01005c:	0ded0d13          	addi	s10,s10,222 # 1c010136 <pos_set_slave_stack>
1c010060:	001d6d13          	ori	s10,s10,1
1c010064:	30045073          	csrwi	mstatus,8
1c010068:	a829                	j	1c010082 <pos_master_loop>

1c01006a <pos_master_event>:
1c01006a:	018b2b03          	lw	s6,24(s6)
1c01006e:	000b0a63          	beqz	s6,1c010082 <pos_master_loop>

1c010072 <pos_push_event_to_fc_retry>:
1c010072:	000ba283          	lw	t0,0(s7)
1c010076:	06029a63          	bnez	t0,1c0100ea <pos_push_event_to_fc_wait>
1c01007a:	016ba023          	sw	s6,0(s7)
1c01007e:	018ca023          	sw	s8,0(s9)

1c010082 <pos_master_loop>:
1c010082:	00042b03          	lw	s6,0(s0)
1c010086:	040b0b63          	beqz	s6,1c0100dc <pos_master_sleep>

1c01008a <pos_master_loop_update_next>:
1c01008a:	020b2e83          	lw	t4,32(s6)
1c01008e:	01d42023          	sw	t4,0(s0)
1c010092:	020b2f03          	lw	t5,32(s6)
1c010096:	ffee9ae3          	bne	t4,t5,1c01008a <pos_master_loop_update_next>

1c01009a <pos_master_loop_exec_task>:
1c01009a:	004b2503          	lw	a0,4(s6)
1c01009e:	000b2283          	lw	t0,0(s6)
1c0100a2:	008b2103          	lw	sp,8(s6)
1c0100a6:	00cb2303          	lw	t1,12(s6)
1c0100aa:	010b2383          	lw	t2,16(s6)
1c0100ae:	024b2f03          	lw	t5,36(s6)
1c0100b2:	014b2f83          	lw	t6,20(s6)
1c0100b6:	80d6                	mv	ra,s5
1c0100b8:	911a                	add	sp,sp,t1
1c0100ba:	01f02e23          	sw	t6,28(zero) # 1c <pos_cluster_nb_active_pe>

1c0100be <pos_no_stack_check>:
1c0100be:	09e9a223          	sw	t5,132(s3) # 204084 <__l1_heap_size+0x1c40a4>
1c0100c2:	21e9a023          	sw	t5,512(s3)
1c0100c6:	21e9a623          	sw	t5,524(s3)

1c0100ca <pos_master_no_slave_barrier>:
1c0100ca:	000f2863          	p.beqimm	t5,0,1c0100da <pos_master_loop_no_slave>
1c0100ce:	09a9a023          	sw	s10,128(s3)
1c0100d2:	0879a023          	sw	t2,128(s3)
1c0100d6:	0829a023          	sw	sp,128(s3)

1c0100da <pos_master_loop_no_slave>:
1c0100da:	8282                	jr	t0

1c0100dc <pos_master_sleep>:
1c0100dc:	0149a423          	sw	s4,8(s3)
1c0100e0:	03c9e003          	p.elw	zero,60(s3)
1c0100e4:	0149a223          	sw	s4,4(s3)
1c0100e8:	bf69                	j	1c010082 <pos_master_loop>

1c0100ea <pos_push_event_to_fc_wait>:
1c0100ea:	0149a423          	sw	s4,8(s3)
1c0100ee:	03c9e003          	p.elw	zero,60(s3)
1c0100f2:	0149a223          	sw	s4,4(s3)
1c0100f6:	bfb5                	j	1c010072 <pos_push_event_to_fc_retry>

1c0100f8 <pos_slave_start>:
1c0100f8:	00204937          	lui	s2,0x204
1c0100fc:	f14029f3          	csrr	s3,mhartid
1c010100:	01f9f993          	andi	s3,s3,31
1c010104:	00000a17          	auipc	s4,0x0
1c010108:	012a0a13          	addi	s4,s4,18 # 1c010116 <pos_fork_return>
1c01010c:	00000a97          	auipc	s5,0x0
1c010110:	00ea8a93          	addi	s5,s5,14 # 1c01011a <pos_wait_for_dispatch>
1c010114:	a019                	j	1c01011a <pos_wait_for_dispatch>

1c010116 <pos_fork_return>:
1c010116:	21c96283          	p.elw	t0,540(s2) # 20421c <__l1_heap_size+0x1c423c>

1c01011a <pos_wait_for_dispatch>:
1c01011a:	08096283          	p.elw	t0,128(s2)
1c01011e:	08096503          	p.elw	a0,128(s2)
1c010122:	0012f313          	andi	t1,t0,1
1c010126:	00031563          	bnez	t1,1c010130 <pos_other_entry>

1c01012a <pos_fork_entry>:
1c01012a:	000a00b3          	add	ra,s4,zero
1c01012e:	8282                	jr	t0

1c010130 <pos_other_entry>:
1c010130:	000a80b3          	add	ra,s5,zero
1c010134:	8282                	jr	t0

1c010136 <pos_set_slave_stack>:
1c010136:	08096283          	p.elw	t0,128(s2)
1c01013a:	00098f13          	mv	t5,s3
1c01013e:	02af0eb3          	mul	t4,t5,a0
1c010142:	005e8133          	add	sp,t4,t0
1c010146:	8082                	ret

1c010148 <min_results_L2>:
	...

1c0101ac <max_results_L2>:
	...

1c010210 <Image_L2>:
1c010210:	0000 0001 0002 0003 0004 0005 0006 0007     ................
1c010220:	0008 0009 000a 000b 000c 000d 000e 000f     ................
1c010230:	0010 0011 0012 0013 0014 0015 0016 0017     ................
1c010240:	0018 0019 001a 001b 001c 001d 001e 001f     ................
1c010250:	0020 0021 0022 0023 0024 0025 0026 0027      .!.".#.$.%.&.'.
1c010260:	0028 0029 002a 002b 002c 002d 002e 002f     (.).*.+.,.-.../.
1c010270:	0030 0031 0032 0033 0034 0035 0036 0037     0.1.2.3.4.5.6.7.
1c010280:	0038 0039 003a 003b 003c 003d 003e 003f     8.9.:.;.<.=.>.?.
1c010290:	0040 0041 0042 0043 0044 0045 0046 0047     @.A.B.C.D.E.F.G.
1c0102a0:	0048 0049 004a 004b 004c 004d 004e 004f     H.I.J.K.L.M.N.O.
1c0102b0:	0050 0000                                   P...

1c0102b4 <pos_libc_uart_buffer_cl>:
	...

1c010334 <pos_libc_uart_buffer>:
	...

1c0103b4 <pos_uart>:
	...
