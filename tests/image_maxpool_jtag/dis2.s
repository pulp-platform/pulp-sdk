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
1c008084:	5c250513          	addi	a0,a0,1474 # 1c00b642 <pos_init_entry>
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

1c008712 <cpu_perf_get>:
1c008712:	10f52863          	p.beqimm	a0,15,1c008822 <cpu_perf_get+0x110>
1c008716:	47bd                	li	a5,15
1c008718:	02a7e363          	bltu	a5,a0,1c00873e <cpu_perf_get+0x2c>
1c00871c:	14752d63          	p.beqimm	a0,7,1c008876 <cpu_perf_get+0x164>
1c008720:	479d                	li	a5,7
1c008722:	0aa7f963          	bgeu	a5,a0,1c0087d4 <cpu_perf_get+0xc2>
1c008726:	14b52563          	p.beqimm	a0,11,1c008870 <cpu_perf_get+0x15e>
1c00872a:	47ad                	li	a5,11
1c00872c:	08a7fd63          	bgeu	a5,a0,1c0087c6 <cpu_perf_get+0xb4>
1c008730:	12d52d63          	p.beqimm	a0,13,1c00886a <cpu_perf_get+0x158>
1c008734:	02e53863          	p.bneimm	a0,14,1c008764 <cpu_perf_get+0x52>
1c008738:	78e02573          	csrr	a0,0x78e
1c00873c:	8082                	ret
1c00873e:	47e1                	li	a5,24
1c008740:	12f50263          	beq	a0,a5,1c008864 <cpu_perf_get+0x152>
1c008744:	02a7fe63          	bgeu	a5,a0,1c008780 <cpu_perf_get+0x6e>
1c008748:	47f1                	li	a5,28
1c00874a:	10f50a63          	beq	a0,a5,1c00885e <cpu_perf_get+0x14c>
1c00874e:	02a7f063          	bgeu	a5,a0,1c00876e <cpu_perf_get+0x5c>
1c008752:	47f5                	li	a5,29
1c008754:	10f50263          	beq	a0,a5,1c008858 <cpu_perf_get+0x146>
1c008758:	47f9                	li	a5,30
1c00875a:	12f51163          	bne	a0,a5,1c00887c <cpu_perf_get+0x16a>
1c00875e:	79e02573          	csrr	a0,0x79e
1c008762:	8082                	ret
1c008764:	10c53c63          	p.bneimm	a0,12,1c00887c <cpu_perf_get+0x16a>
1c008768:	78c02573          	csrr	a0,0x78c
1c00876c:	8082                	ret
1c00876e:	47e9                	li	a5,26
1c008770:	0ef50163          	beq	a0,a5,1c008852 <cpu_perf_get+0x140>
1c008774:	47ed                	li	a5,27
1c008776:	02f51363          	bne	a0,a5,1c00879c <cpu_perf_get+0x8a>
1c00877a:	79b02573          	csrr	a0,0x79b
1c00877e:	8082                	ret
1c008780:	47d1                	li	a5,20
1c008782:	0cf50563          	beq	a0,a5,1c00884c <cpu_perf_get+0x13a>
1c008786:	02a7f763          	bgeu	a5,a0,1c0087b4 <cpu_perf_get+0xa2>
1c00878a:	47d9                	li	a5,22
1c00878c:	0af50d63          	beq	a0,a5,1c008846 <cpu_perf_get+0x134>
1c008790:	47dd                	li	a5,23
1c008792:	00f51b63          	bne	a0,a5,1c0087a8 <cpu_perf_get+0x96>
1c008796:	79702573          	csrr	a0,0x797
1c00879a:	8082                	ret
1c00879c:	47e5                	li	a5,25
1c00879e:	0cf51f63          	bne	a0,a5,1c00887c <cpu_perf_get+0x16a>
1c0087a2:	79902573          	csrr	a0,0x799
1c0087a6:	8082                	ret
1c0087a8:	47d5                	li	a5,21
1c0087aa:	0cf51963          	bne	a0,a5,1c00887c <cpu_perf_get+0x16a>
1c0087ae:	79502573          	csrr	a0,0x795
1c0087b2:	8082                	ret
1c0087b4:	47c9                	li	a5,18
1c0087b6:	08f50563          	beq	a0,a5,1c008840 <cpu_perf_get+0x12e>
1c0087ba:	47cd                	li	a5,19
1c0087bc:	02f51863          	bne	a0,a5,1c0087ec <cpu_perf_get+0xda>
1c0087c0:	79302573          	csrr	a0,0x793
1c0087c4:	8082                	ret
1c0087c6:	06952a63          	p.beqimm	a0,9,1c00883a <cpu_perf_get+0x128>
1c0087ca:	02a53763          	p.bneimm	a0,10,1c0087f8 <cpu_perf_get+0xe6>
1c0087ce:	78a02573          	csrr	a0,0x78a
1c0087d2:	8082                	ret
1c0087d4:	06352063          	p.beqimm	a0,3,1c008834 <cpu_perf_get+0x122>
1c0087d8:	478d                	li	a5,3
1c0087da:	02a7f963          	bgeu	a5,a0,1c00880c <cpu_perf_get+0xfa>
1c0087de:	04552863          	p.beqimm	a0,5,1c00882e <cpu_perf_get+0x11c>
1c0087e2:	02653063          	p.bneimm	a0,6,1c008802 <cpu_perf_get+0xf0>
1c0087e6:	78602573          	csrr	a0,0x786
1c0087ea:	8082                	ret
1c0087ec:	47c1                	li	a5,16
1c0087ee:	08f51763          	bne	a0,a5,1c00887c <cpu_perf_get+0x16a>
1c0087f2:	79002573          	csrr	a0,0x790
1c0087f6:	8082                	ret
1c0087f8:	08853263          	p.bneimm	a0,8,1c00887c <cpu_perf_get+0x16a>
1c0087fc:	78802573          	csrr	a0,0x788
1c008800:	8082                	ret
1c008802:	06453d63          	p.bneimm	a0,4,1c00887c <cpu_perf_get+0x16a>
1c008806:	78402573          	csrr	a0,0x784
1c00880a:	8082                	ret
1c00880c:	00152e63          	p.beqimm	a0,1,1c008828 <cpu_perf_get+0x116>
1c008810:	00253563          	p.bneimm	a0,2,1c00881a <cpu_perf_get+0x108>
1c008814:	78202573          	csrr	a0,0x782
1c008818:	8082                	ret
1c00881a:	e12d                	bnez	a0,1c00887c <cpu_perf_get+0x16a>
1c00881c:	78002573          	csrr	a0,0x780
1c008820:	8082                	ret
1c008822:	78f02573          	csrr	a0,0x78f
1c008826:	8082                	ret
1c008828:	78102573          	csrr	a0,0x781
1c00882c:	8082                	ret
1c00882e:	78502573          	csrr	a0,0x785
1c008832:	8082                	ret
1c008834:	78302573          	csrr	a0,0x783
1c008838:	8082                	ret
1c00883a:	78902573          	csrr	a0,0x789
1c00883e:	8082                	ret
1c008840:	79202573          	csrr	a0,0x792
1c008844:	8082                	ret
1c008846:	79602573          	csrr	a0,0x796
1c00884a:	8082                	ret
1c00884c:	79402573          	csrr	a0,0x794
1c008850:	8082                	ret
1c008852:	79a02573          	csrr	a0,0x79a
1c008856:	8082                	ret
1c008858:	79d02573          	csrr	a0,0x79d
1c00885c:	8082                	ret
1c00885e:	79c02573          	csrr	a0,0x79c
1c008862:	8082                	ret
1c008864:	79802573          	csrr	a0,0x798
1c008868:	8082                	ret
1c00886a:	78d02573          	csrr	a0,0x78d
1c00886e:	8082                	ret
1c008870:	78b02573          	csrr	a0,0x78b
1c008874:	8082                	ret
1c008876:	78702573          	csrr	a0,0x787
1c00887a:	8082                	ret
1c00887c:	4501                	li	a0,0
1c00887e:	8082                	ret

1c008880 <maxpool_benchmark_simd_3x3>:
1c008880:	1c0108b7          	lui	a7,0x1c010
1c008884:	1141                	addi	sp,sp,-16
1c008886:	14888893          	addi	a7,a7,328 # 1c010148 <min_results_L2>
1c00888a:	1c010eb7          	lui	t4,0x1c010
1c00888e:	6e41                	lui	t3,0x10
1c008890:	c622                	sw	s0,12(sp)
1c008892:	06488313          	addi	t1,a7,100
1c008896:	4801                	li	a6,0
1c008898:	4581                	li	a1,0
1c00889a:	210e8e93          	addi	t4,t4,528 # 1c010210 <Image_L2>
1c00889e:	1e7d                	addi	t3,t3,-1
1c0088a0:	4f19                	li	t5,6
1c0088a2:	03100513          	li	a0,49
1c0088a6:	00381793          	slli	a5,a6,0x3
1c0088aa:	97c2                	add	a5,a5,a6
1c0088ac:	97ae                	add	a5,a5,a1
1c0088ae:	0786                	slli	a5,a5,0x1
1c0088b0:	97f6                	add	a5,a5,t4
1c0088b2:	0027d403          	lhu	s0,2(a5) # 10002 <pos_soc_event_callback+0xfbbe>
1c0088b6:	0007d703          	lhu	a4,0(a5)
1c0088ba:	0107d383          	lhu	t2,16(a5)
1c0088be:	0207d683          	lhu	a3,32(a5)
1c0088c2:	d0e40757          	pv.pack	a4,s0,a4
1c0088c6:	0047d283          	lhu	t0,4(a5)
1c0088ca:	00e7d603          	lhu	a2,14(a5)
1c0088ce:	01e7df83          	lhu	t6,30(a5)
1c0088d2:	00c7d403          	lhu	s0,12(a5)
1c0088d6:	d0c38657          	pv.pack	a2,t2,a2
1c0088da:	d1f68fd7          	pv.pack	t6,a3,t6
1c0088de:	d05402d7          	pv.pack	t0,s0,t0
1c0088e2:	39f603d7          	pv.maxu.h	t2,a2,t6
1c0088e6:	385706d7          	pv.maxu.h	a3,a4,t0
1c0088ea:	387686d7          	pv.maxu.h	a3,a3,t2
1c0088ee:	28570757          	pv.minu.h	a4,a4,t0
1c0088f2:	29f60657          	pv.minu.h	a2,a2,t6
1c0088f6:	0227df83          	lhu	t6,34(a5)
1c0088fa:	28c707d7          	pv.minu.h	a5,a4,a2
1c0088fe:	01069713          	slli	a4,a3,0x10
1c008902:	8341                	srli	a4,a4,0x10
1c008904:	82c1                	srli	a3,a3,0x10
1c008906:	04d76733          	p.max	a4,a4,a3
1c00890a:	01079693          	slli	a3,a5,0x10
1c00890e:	82c1                	srli	a3,a3,0x10
1c008910:	83c1                	srli	a5,a5,0x10
1c008912:	04f6d7b3          	p.minu	a5,a3,a5
1c008916:	01c77733          	and	a4,a4,t3
1c00891a:	05f76733          	p.max	a4,a4,t6
1c00891e:	04ffdfb3          	p.minu	t6,t6,a5
1c008922:	01f8912b          	p.sh	t6,2(a7!)
1c008926:	00158793          	addi	a5,a1,1
1c00892a:	00e3112b          	p.sh	a4,2(t1!) # 1000002 <__l1_heap_size+0xfc0022>
1c00892e:	00ff5963          	bge	t5,a5,1c008940 <maxpool_benchmark_simd_3x3+0xc0>
1c008932:	15e9                	addi	a1,a1,-6
1c008934:	0805                	addi	a6,a6,1
1c008936:	157d                	addi	a0,a0,-1
1c008938:	f53d                	bnez	a0,1c0088a6 <maxpool_benchmark_simd_3x3+0x26>
1c00893a:	4432                	lw	s0,12(sp)
1c00893c:	0141                	addi	sp,sp,16
1c00893e:	8082                	ret
1c008940:	85be                	mv	a1,a5
1c008942:	157d                	addi	a0,a0,-1
1c008944:	f12d                	bnez	a0,1c0088a6 <maxpool_benchmark_simd_3x3+0x26>
1c008946:	bfd5                	j	1c00893a <maxpool_benchmark_simd_3x3+0xba>

1c008948 <perf_print_all_fixed>:
1c008948:	1141                	addi	sp,sp,-16
1c00894a:	c422                	sw	s0,8(sp)
1c00894c:	c606                	sw	ra,12(sp)
1c00894e:	f1402473          	csrr	s0,mhartid
1c008952:	780025f3          	csrr	a1,0x780
1c008956:	1c001537          	lui	a0,0x1c001
1c00895a:	96850513          	addi	a0,a0,-1688 # 1c000968 <__clz_tab+0x100>
1c00895e:	2ba9                	jal	1c008eb8 <printf>
1c008960:	781025f3          	csrr	a1,0x781
1c008964:	1c001537          	lui	a0,0x1c001
1c008968:	97c50513          	addi	a0,a0,-1668 # 1c00097c <__clz_tab+0x114>
1c00896c:	23b1                	jal	1c008eb8 <printf>
1c00896e:	78a025f3          	csrr	a1,0x78a
1c008972:	1c001537          	lui	a0,0x1c001
1c008976:	99050513          	addi	a0,a0,-1648 # 1c000990 <__clz_tab+0x128>
1c00897a:	2b3d                	jal	1c008eb8 <printf>
1c00897c:	782025f3          	csrr	a1,0x782
1c008980:	1c001537          	lui	a0,0x1c001
1c008984:	9a450513          	addi	a0,a0,-1628 # 1c0009a4 <__clz_tab+0x13c>
1c008988:	2b05                	jal	1c008eb8 <printf>
1c00898a:	783025f3          	csrr	a1,0x783
1c00898e:	1c001537          	lui	a0,0x1c001
1c008992:	9b850513          	addi	a0,a0,-1608 # 1c0009b8 <__clz_tab+0x150>
1c008996:	230d                	jal	1c008eb8 <printf>
1c008998:	784025f3          	csrr	a1,0x784
1c00899c:	1c001537          	lui	a0,0x1c001
1c0089a0:	9cc50513          	addi	a0,a0,-1588 # 1c0009cc <__clz_tab+0x164>
1c0089a4:	2b11                	jal	1c008eb8 <printf>
1c0089a6:	785025f3          	csrr	a1,0x785
1c0089aa:	1c001537          	lui	a0,0x1c001
1c0089ae:	9e050513          	addi	a0,a0,-1568 # 1c0009e0 <__clz_tab+0x178>
1c0089b2:	2319                	jal	1c008eb8 <printf>
1c0089b4:	786025f3          	csrr	a1,0x786
1c0089b8:	1c001537          	lui	a0,0x1c001
1c0089bc:	9f050513          	addi	a0,a0,-1552 # 1c0009f0 <__clz_tab+0x188>
1c0089c0:	29e5                	jal	1c008eb8 <printf>
1c0089c2:	787025f3          	csrr	a1,0x787
1c0089c6:	1c001537          	lui	a0,0x1c001
1c0089ca:	a0050513          	addi	a0,a0,-1536 # 1c000a00 <__clz_tab+0x198>
1c0089ce:	21ed                	jal	1c008eb8 <printf>
1c0089d0:	788025f3          	csrr	a1,0x788
1c0089d4:	1c001537          	lui	a0,0x1c001
1c0089d8:	a1050513          	addi	a0,a0,-1520 # 1c000a10 <__clz_tab+0x1a8>
1c0089dc:	29f1                	jal	1c008eb8 <printf>
1c0089de:	789025f3          	csrr	a1,0x789
1c0089e2:	1c001537          	lui	a0,0x1c001
1c0089e6:	a2450513          	addi	a0,a0,-1500 # 1c000a24 <__clz_tab+0x1bc>
1c0089ea:	21f9                	jal	1c008eb8 <printf>
1c0089ec:	ca5417b3          	p.extractu	a5,s0,5,5
1c0089f0:	477d                	li	a4,31
1c0089f2:	00e79663          	bne	a5,a4,1c0089fe <perf_print_all_fixed+0xb6>
1c0089f6:	40b2                	lw	ra,12(sp)
1c0089f8:	4422                	lw	s0,8(sp)
1c0089fa:	0141                	addi	sp,sp,16
1c0089fc:	8082                	ret
1c0089fe:	78b025f3          	csrr	a1,0x78b
1c008a02:	1c001537          	lui	a0,0x1c001
1c008a06:	a3850513          	addi	a0,a0,-1480 # 1c000a38 <__clz_tab+0x1d0>
1c008a0a:	217d                	jal	1c008eb8 <printf>
1c008a0c:	4531                	li	a0,12
1c008a0e:	3311                	jal	1c008712 <cpu_perf_get>
1c008a10:	85aa                	mv	a1,a0
1c008a12:	1c001537          	lui	a0,0x1c001
1c008a16:	a5850513          	addi	a0,a0,-1448 # 1c000a58 <__clz_tab+0x1f0>
1c008a1a:	2979                	jal	1c008eb8 <printf>
1c008a1c:	ca5417b3          	p.extractu	a5,s0,5,5
1c008a20:	477d                	li	a4,31
1c008a22:	0ae78963          	beq	a5,a4,1c008ad4 <perf_print_all_fixed+0x18c>
1c008a26:	78d025f3          	csrr	a1,0x78d
1c008a2a:	1c001537          	lui	a0,0x1c001
1c008a2e:	a7050513          	addi	a0,a0,-1424 # 1c000a70 <__clz_tab+0x208>
1c008a32:	2159                	jal	1c008eb8 <printf>
1c008a34:	4539                	li	a0,14
1c008a36:	39f1                	jal	1c008712 <cpu_perf_get>
1c008a38:	85aa                	mv	a1,a0
1c008a3a:	1c001537          	lui	a0,0x1c001
1c008a3e:	a8850513          	addi	a0,a0,-1400 # 1c000a88 <__clz_tab+0x220>
1c008a42:	299d                	jal	1c008eb8 <printf>
1c008a44:	ca5417b3          	p.extractu	a5,s0,5,5
1c008a48:	477d                	li	a4,31
1c008a4a:	08e78163          	beq	a5,a4,1c008acc <perf_print_all_fixed+0x184>
1c008a4e:	78f025f3          	csrr	a1,0x78f
1c008a52:	1c001537          	lui	a0,0x1c001
1c008a56:	aa450513          	addi	a0,a0,-1372 # 1c000aa4 <__clz_tab+0x23c>
1c008a5a:	29b9                	jal	1c008eb8 <printf>
1c008a5c:	ca5417b3          	p.extractu	a5,s0,5,5
1c008a60:	477d                	li	a4,31
1c008a62:	06e78163          	beq	a5,a4,1c008ac4 <perf_print_all_fixed+0x17c>
1c008a66:	790025f3          	csrr	a1,0x790
1c008a6a:	1c001537          	lui	a0,0x1c001
1c008a6e:	ac050513          	addi	a0,a0,-1344 # 1c000ac0 <__clz_tab+0x258>
1c008a72:	2199                	jal	1c008eb8 <printf>
1c008a74:	ca541433          	p.extractu	s0,s0,5,5
1c008a78:	47fd                	li	a5,31
1c008a7a:	04f40063          	beq	s0,a5,1c008aba <perf_print_all_fixed+0x172>
1c008a7e:	102005b7          	lui	a1,0x10200
1c008a82:	40058593          	addi	a1,a1,1024 # 10200400 <__l1_end+0x2003e0>
1c008a86:	0085a583          	lw	a1,8(a1)
1c008a8a:	1c001537          	lui	a0,0x1c001
1c008a8e:	ad850513          	addi	a0,a0,-1320 # 1c000ad8 <__clz_tab+0x270>
1c008a92:	211d                	jal	1c008eb8 <printf>
1c008a94:	4549                	li	a0,18
1c008a96:	39b5                	jal	1c008712 <cpu_perf_get>
1c008a98:	85aa                	mv	a1,a0
1c008a9a:	1c001537          	lui	a0,0x1c001
1c008a9e:	afc50513          	addi	a0,a0,-1284 # 1c000afc <__clz_tab+0x294>
1c008aa2:	2919                	jal	1c008eb8 <printf>
1c008aa4:	4549                	li	a0,18
1c008aa6:	31b5                	jal	1c008712 <cpu_perf_get>
1c008aa8:	4422                	lw	s0,8(sp)
1c008aaa:	40b2                	lw	ra,12(sp)
1c008aac:	85aa                	mv	a1,a0
1c008aae:	1c001537          	lui	a0,0x1c001
1c008ab2:	b1850513          	addi	a0,a0,-1256 # 1c000b18 <__clz_tab+0x2b0>
1c008ab6:	0141                	addi	sp,sp,16
1c008ab8:	a101                	j	1c008eb8 <printf>
1c008aba:	1a10b5b7          	lui	a1,0x1a10b
1c008abe:	0085a583          	lw	a1,8(a1) # 1a10b008 <__l1_end+0xa10afe8>
1c008ac2:	b7e1                	j	1c008a8a <perf_print_all_fixed+0x142>
1c008ac4:	4541                	li	a0,16
1c008ac6:	31b1                	jal	1c008712 <cpu_perf_get>
1c008ac8:	85aa                	mv	a1,a0
1c008aca:	b745                	j	1c008a6a <perf_print_all_fixed+0x122>
1c008acc:	453d                	li	a0,15
1c008ace:	3191                	jal	1c008712 <cpu_perf_get>
1c008ad0:	85aa                	mv	a1,a0
1c008ad2:	b741                	j	1c008a52 <perf_print_all_fixed+0x10a>
1c008ad4:	4535                	li	a0,13
1c008ad6:	3935                	jal	1c008712 <cpu_perf_get>
1c008ad8:	85aa                	mv	a1,a0
1c008ada:	bf81                	j	1c008a2a <perf_print_all_fixed+0xe2>

1c008adc <run_benchmark_on_fc>:
1c008adc:	1141                	addi	sp,sp,-16
1c008ade:	87aa                	mv	a5,a0
1c008ae0:	1c001537          	lui	a0,0x1c001
1c008ae4:	c226                	sw	s1,4(sp)
1c008ae6:	b3450513          	addi	a0,a0,-1228 # 1c000b34 <__clz_tab+0x2cc>
1c008aea:	84ae                	mv	s1,a1
1c008aec:	85be                	mv	a1,a5
1c008aee:	c606                	sw	ra,12(sp)
1c008af0:	c422                	sw	s0,8(sp)
1c008af2:	26d9                	jal	1c008eb8 <printf>
1c008af4:	800007b7          	lui	a5,0x80000
1c008af8:	fff7c793          	not	a5,a5
1c008afc:	cc079073          	csrw	0xcc0,a5
1c008b00:	f1402473          	csrr	s0,mhartid
1c008b04:	47fd                	li	a5,31
1c008b06:	ca541733          	p.extractu	a4,s0,5,5
1c008b0a:	08f70363          	beq	a4,a5,1c008b90 <run_benchmark_on_fc+0xb4>
1c008b0e:	102007b7          	lui	a5,0x10200
1c008b12:	4705                	li	a4,1
1c008b14:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c008b18:	02e7a023          	sw	a4,32(a5)
1c008b1c:	4781                	li	a5,0
1c008b1e:	79f79073          	csrw	0x79f,a5
1c008b22:	477d                	li	a4,31
1c008b24:	ca5417b3          	p.extractu	a5,s0,5,5
1c008b28:	08e78163          	beq	a5,a4,1c008baa <run_benchmark_on_fc+0xce>
1c008b2c:	102007b7          	lui	a5,0x10200
1c008b30:	4705                	li	a4,1
1c008b32:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c008b36:	00e7ac23          	sw	a4,24(a5)
1c008b3a:	478d                	li	a5,3
1c008b3c:	cc179073          	csrw	0xcc1,a5
1c008b40:	9482                	jalr	s1
1c008b42:	ca5417b3          	p.extractu	a5,s0,5,5
1c008b46:	477d                	li	a4,31
1c008b48:	06e78f63          	beq	a5,a4,1c008bc6 <run_benchmark_on_fc+0xea>
1c008b4c:	102007b7          	lui	a5,0x10200
1c008b50:	40078793          	addi	a5,a5,1024 # 10200400 <__l1_end+0x2003e0>
1c008b54:	0007a023          	sw	zero,0(a5)
1c008b58:	4781                	li	a5,0
1c008b5a:	cc179073          	csrw	0xcc1,a5
1c008b5e:	33ed                	jal	1c008948 <perf_print_all_fixed>
1c008b60:	ca541433          	p.extractu	s0,s0,5,5
1c008b64:	47fd                	li	a5,31
1c008b66:	06f40d63          	beq	s0,a5,1c008be0 <run_benchmark_on_fc+0x104>
1c008b6a:	102005b7          	lui	a1,0x10200
1c008b6e:	40058593          	addi	a1,a1,1024 # 10200400 <__l1_end+0x2003e0>
1c008b72:	0085a583          	lw	a1,8(a1)
1c008b76:	4422                	lw	s0,8(sp)
1c008b78:	40b2                	lw	ra,12(sp)
1c008b7a:	4492                	lw	s1,4(sp)
1c008b7c:	00259793          	slli	a5,a1,0x2
1c008b80:	95be                	add	a1,a1,a5
1c008b82:	1c001537          	lui	a0,0x1c001
1c008b86:	058a                	slli	a1,a1,0x2
1c008b88:	b5050513          	addi	a0,a0,-1200 # 1c000b50 <__clz_tab+0x2e8>
1c008b8c:	0141                	addi	sp,sp,16
1c008b8e:	a62d                	j	1c008eb8 <printf>
1c008b90:	4785                	li	a5,1
1c008b92:	1a10b737          	lui	a4,0x1a10b
1c008b96:	02f72023          	sw	a5,32(a4) # 1a10b020 <__l1_end+0xa10b000>
1c008b9a:	4781                	li	a5,0
1c008b9c:	79f79073          	csrw	0x79f,a5
1c008ba0:	477d                	li	a4,31
1c008ba2:	ca5417b3          	p.extractu	a5,s0,5,5
1c008ba6:	f8e793e3          	bne	a5,a4,1c008b2c <run_benchmark_on_fc+0x50>
1c008baa:	4785                	li	a5,1
1c008bac:	1a10b737          	lui	a4,0x1a10b
1c008bb0:	00f72c23          	sw	a5,24(a4) # 1a10b018 <__l1_end+0xa10aff8>
1c008bb4:	478d                	li	a5,3
1c008bb6:	cc179073          	csrw	0xcc1,a5
1c008bba:	9482                	jalr	s1
1c008bbc:	ca5417b3          	p.extractu	a5,s0,5,5
1c008bc0:	477d                	li	a4,31
1c008bc2:	f8e795e3          	bne	a5,a4,1c008b4c <run_benchmark_on_fc+0x70>
1c008bc6:	1a10b7b7          	lui	a5,0x1a10b
1c008bca:	0007a023          	sw	zero,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c008bce:	4781                	li	a5,0
1c008bd0:	cc179073          	csrw	0xcc1,a5
1c008bd4:	3b95                	jal	1c008948 <perf_print_all_fixed>
1c008bd6:	ca541433          	p.extractu	s0,s0,5,5
1c008bda:	47fd                	li	a5,31
1c008bdc:	f8f417e3          	bne	s0,a5,1c008b6a <run_benchmark_on_fc+0x8e>
1c008be0:	1a10b5b7          	lui	a1,0x1a10b
1c008be4:	0085a583          	lw	a1,8(a1) # 1a10b008 <__l1_end+0xa10afe8>
1c008be8:	b779                	j	1c008b76 <run_benchmark_on_fc+0x9a>

1c008bea <main>:
1c008bea:	f14027f3          	csrr	a5,mhartid
1c008bee:	ca5797b3          	p.extractu	a5,a5,5,5
1c008bf2:	e399                	bnez	a5,1c008bf8 <main+0xe>
1c008bf4:	4501                	li	a0,0
1c008bf6:	8082                	ret
1c008bf8:	1141                	addi	sp,sp,-16
1c008bfa:	45e5                	li	a1,25
1c008bfc:	02000513          	li	a0,32
1c008c00:	c606                	sw	ra,12(sp)
1c008c02:	2089                	jal	1c008c44 <padctrl_mode_set>
1c008c04:	45e9                	li	a1,26
1c008c06:	02100513          	li	a0,33
1c008c0a:	282d                	jal	1c008c44 <padctrl_mode_set>
1c008c0c:	1c001537          	lui	a0,0x1c001
1c008c10:	ba050513          	addi	a0,a0,-1120 # 1c000ba0 <__clz_tab+0x338>
1c008c14:	2455                	jal	1c008eb8 <printf>
1c008c16:	1c0095b7          	lui	a1,0x1c009
1c008c1a:	1c001537          	lui	a0,0x1c001
1c008c1e:	bbc50513          	addi	a0,a0,-1092 # 1c000bbc <__clz_tab+0x354>
1c008c22:	88058593          	addi	a1,a1,-1920 # 1c008880 <maxpool_benchmark_simd_3x3>
1c008c26:	3d5d                	jal	1c008adc <run_benchmark_on_fc>
1c008c28:	40b2                	lw	ra,12(sp)
1c008c2a:	4501                	li	a0,0
1c008c2c:	0141                	addi	sp,sp,16
1c008c2e:	8082                	ret

1c008c30 <pos_soc_init>:
1c008c30:	02faf737          	lui	a4,0x2faf
1c008c34:	00400793          	li	a5,4
1c008c38:	08070713          	addi	a4,a4,128 # 2faf080 <__l1_heap_size+0x2f6f0a0>
1c008c3c:	c398                	sw	a4,0(a5)
1c008c3e:	c798                	sw	a4,8(a5)
1c008c40:	c3d8                	sw	a4,4(a5)
1c008c42:	8082                	ret

1c008c44 <padctrl_mode_set>:
1c008c44:	034287b7          	lui	a5,0x3428
1c008c48:	953e                	add	a0,a0,a5
1c008c4a:	050e                	slli	a0,a0,0x3
1c008c4c:	415c                	lw	a5,4(a0)
1c008c4e:	0511                	addi	a0,a0,4
1c008c50:	c805a7b3          	p.insert	a5,a1,4,0
1c008c54:	c11c                	sw	a5,0(a0)
1c008c56:	8082                	ret

1c008c58 <pos_libc_putc_uart_req>:
1c008c58:	410c                	lw	a1,0(a0)
1c008c5a:	1141                	addi	sp,sp,-16
1c008c5c:	c422                	sw	s0,8(sp)
1c008c5e:	00759793          	slli	a5,a1,0x7
1c008c62:	1c0105b7          	lui	a1,0x1c010
1c008c66:	2b458593          	addi	a1,a1,692 # 1c0102b4 <pos_libc_uart_buffer_cl>
1c008c6a:	c606                	sw	ra,12(sp)
1c008c6c:	95be                	add	a1,a1,a5
1c008c6e:	0005c783          	lbu	a5,0(a1)
1c008c72:	842a                	mv	s0,a0
1c008c74:	cf95                	beqz	a5,1c008cb0 <pos_libc_putc_uart_req+0x58>
1c008c76:	00158793          	addi	a5,a1,1
1c008c7a:	0007c703          	lbu	a4,0(a5) # 3428000 <__l1_heap_size+0x33e8020>
1c008c7e:	863e                	mv	a2,a5
1c008c80:	0785                	addi	a5,a5,1
1c008c82:	ff65                	bnez	a4,1c008c7a <pos_libc_putc_uart_req+0x22>
1c008c84:	8e0d                	sub	a2,a2,a1
1c008c86:	01000513          	li	a0,16
1c008c8a:	025020ef          	jal	ra,1c00b4ae <pi_uart_write>
1c008c8e:	401c                	lw	a5,0(s0)
1c008c90:	4705                	li	a4,1
1c008c92:	c058                	sw	a4,4(s0)
1c008c94:	04078793          	addi	a5,a5,64
1c008c98:	00201737          	lui	a4,0x201
1c008c9c:	40b2                	lw	ra,12(sp)
1c008c9e:	4422                	lw	s0,8(sp)
1c008ca0:	07da                	slli	a5,a5,0x16
1c008ca2:	e0470713          	addi	a4,a4,-508 # 200e04 <__l1_heap_size+0x1c0e24>
1c008ca6:	97ba                	add	a5,a5,a4
1c008ca8:	0007a023          	sw	zero,0(a5)
1c008cac:	0141                	addi	sp,sp,16
1c008cae:	8082                	ret
1c008cb0:	4601                	li	a2,0
1c008cb2:	bfd1                	j	1c008c86 <pos_libc_putc_uart_req+0x2e>

1c008cb4 <strchr>:
1c008cb4:	00054703          	lbu	a4,0(a0)
1c008cb8:	0ff5f593          	andi	a1,a1,255
1c008cbc:	87aa                	mv	a5,a0
1c008cbe:	00b70863          	beq	a4,a1,1c008cce <strchr+0x1a>
1c008cc2:	cb01                	beqz	a4,1c008cd2 <strchr+0x1e>
1c008cc4:	0017c703          	lbu	a4,1(a5)
1c008cc8:	0785                	addi	a5,a5,1
1c008cca:	feb71ce3          	bne	a4,a1,1c008cc2 <strchr+0xe>
1c008cce:	853e                	mv	a0,a5
1c008cd0:	8082                	ret
1c008cd2:	4501                	li	a0,0
1c008cd4:	dded                	beqz	a1,1c008cce <strchr+0x1a>
1c008cd6:	8082                	ret

1c008cd8 <pos_libc_fputc_locked>:
1c008cd8:	1c0017b7          	lui	a5,0x1c001
1c008cdc:	4107a783          	lw	a5,1040(a5) # 1c001410 <pos_io_uart_enabled>
1c008ce0:	c7a9                	beqz	a5,1c008d2a <pos_libc_fputc_locked+0x52>
1c008ce2:	f1402673          	csrr	a2,mhartid
1c008ce6:	477d                	li	a4,31
1c008ce8:	ca5617b3          	p.extractu	a5,a2,5,5
1c008cec:	08e78063          	beq	a5,a4,1c008d6c <pos_libc_fputc_locked+0x94>
1c008cf0:	00779693          	slli	a3,a5,0x7
1c008cf4:	00279713          	slli	a4,a5,0x2
1c008cf8:	1c0105b7          	lui	a1,0x1c010
1c008cfc:	1c0017b7          	lui	a5,0x1c001
1c008d00:	2b458593          	addi	a1,a1,692 # 1c0102b4 <pos_libc_uart_buffer_cl>
1c008d04:	41878793          	addi	a5,a5,1048 # 1c001418 <pos_libc_uart_buffer_index_cl>
1c008d08:	95b6                	add	a1,a1,a3
1c008d0a:	97ba                	add	a5,a5,a4
1c008d0c:	4398                	lw	a4,0(a5)
1c008d0e:	0ff57513          	andi	a0,a0,255
1c008d12:	07f00813          	li	a6,127
1c008d16:	00170693          	addi	a3,a4,1
1c008d1a:	972e                	add	a4,a4,a1
1c008d1c:	c394                	sw	a3,0(a5)
1c008d1e:	00a70023          	sb	a0,0(a4)
1c008d22:	01068663          	beq	a3,a6,1c008d2e <pos_libc_fputc_locked+0x56>
1c008d26:	00a52463          	p.beqimm	a0,10,1c008d2e <pos_libc_fputc_locked+0x56>
1c008d2a:	4501                	li	a0,0
1c008d2c:	8082                	ret
1c008d2e:	711d                	addi	sp,sp,-96
1c008d30:	ce86                	sw	ra,92(sp)
1c008d32:	96ae                	add	a3,a3,a1
1c008d34:	0007a023          	sw	zero,0(a5)
1c008d38:	00068023          	sb	zero,0(a3)
1c008d3c:	ca561633          	p.extractu	a2,a2,5,5
1c008d40:	47fd                	li	a5,31
1c008d42:	02f61e63          	bne	a2,a5,1c008d7e <pos_libc_fputc_locked+0xa6>
1c008d46:	0005c783          	lbu	a5,0(a1)
1c008d4a:	cba5                	beqz	a5,1c008dba <pos_libc_fputc_locked+0xe2>
1c008d4c:	00158793          	addi	a5,a1,1
1c008d50:	0007c703          	lbu	a4,0(a5)
1c008d54:	863e                	mv	a2,a5
1c008d56:	0785                	addi	a5,a5,1
1c008d58:	ff65                	bnez	a4,1c008d50 <pos_libc_fputc_locked+0x78>
1c008d5a:	8e0d                	sub	a2,a2,a1
1c008d5c:	01000513          	li	a0,16
1c008d60:	74e020ef          	jal	ra,1c00b4ae <pi_uart_write>
1c008d64:	40f6                	lw	ra,92(sp)
1c008d66:	4501                	li	a0,0
1c008d68:	6125                	addi	sp,sp,96
1c008d6a:	8082                	ret
1c008d6c:	1c0017b7          	lui	a5,0x1c001
1c008d70:	1c0105b7          	lui	a1,0x1c010
1c008d74:	41478793          	addi	a5,a5,1044 # 1c001414 <pos_libc_uart_buffer_index>
1c008d78:	33458593          	addi	a1,a1,820 # 1c010334 <pos_libc_uart_buffer>
1c008d7c:	bf41                	j	1c008d0c <pos_libc_fputc_locked+0x34>
1c008d7e:	1c0097b7          	lui	a5,0x1c009
1c008d82:	c5878793          	addi	a5,a5,-936 # 1c008c58 <pos_libc_putc_uart_req>
1c008d86:	ca3e                	sw	a5,20(sp)
1c008d88:	0808                	addi	a0,sp,16
1c008d8a:	003c                	addi	a5,sp,8
1c008d8c:	c432                	sw	a2,8(sp)
1c008d8e:	cc3e                	sw	a5,24(sp)
1c008d90:	c602                	sw	zero,12(sp)
1c008d92:	055020ef          	jal	ra,1c00b5e6 <pos_cluster_push_fc_event>
1c008d96:	46b2                	lw	a3,12(sp)
1c008d98:	002047b7          	lui	a5,0x204
1c008d9c:	4709                	li	a4,2
1c008d9e:	03c78613          	addi	a2,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c008da2:	f2e9                	bnez	a3,1c008d64 <pos_libc_fputc_locked+0x8c>
1c008da4:	00e7a423          	sw	a4,8(a5)
1c008da8:	4214                	lw	a3,0(a2)
1c008daa:	00e7a223          	sw	a4,4(a5)
1c008dae:	46b2                	lw	a3,12(sp)
1c008db0:	daf5                	beqz	a3,1c008da4 <pos_libc_fputc_locked+0xcc>
1c008db2:	40f6                	lw	ra,92(sp)
1c008db4:	4501                	li	a0,0
1c008db6:	6125                	addi	sp,sp,96
1c008db8:	8082                	ret
1c008dba:	4601                	li	a2,0
1c008dbc:	b745                	j	1c008d5c <pos_libc_fputc_locked+0x84>

1c008dbe <pos_libc_prf_locked>:
1c008dbe:	1141                	addi	sp,sp,-16
1c008dc0:	c422                	sw	s0,8(sp)
1c008dc2:	f1402473          	csrr	s0,mhartid
1c008dc6:	c606                	sw	ra,12(sp)
1c008dc8:	47fd                	li	a5,31
1c008dca:	ca541733          	p.extractu	a4,s0,5,5
1c008dce:	02f70963          	beq	a4,a5,1c008e00 <pos_libc_prf_locked+0x42>
1c008dd2:	00400893          	li	a7,4
1c008dd6:	c148c8b3          	p.bset	a7,a7,0,20
1c008dda:	0008a783          	lw	a5,0(a7)
1c008dde:	03f7b163          	p.bneimm	a5,-1,1c008e00 <pos_libc_prf_locked+0x42>
1c008de2:	002047b7          	lui	a5,0x204
1c008de6:	4711                	li	a4,4
1c008de8:	03c78313          	addi	t1,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c008dec:	00e7a423          	sw	a4,8(a5)
1c008df0:	00032803          	lw	a6,0(t1)
1c008df4:	00e7a223          	sw	a4,4(a5)
1c008df8:	0008a803          	lw	a6,0(a7)
1c008dfc:	fff828e3          	p.beqimm	a6,-1,1c008dec <pos_libc_prf_locked+0x2e>
1c008e00:	20dd                	jal	1c008ee6 <pos_libc_prf>
1c008e02:	ca541433          	p.extractu	s0,s0,5,5
1c008e06:	47fd                	li	a5,31
1c008e08:	00f40863          	beq	s0,a5,1c008e18 <pos_libc_prf_locked+0x5a>
1c008e0c:	00002223          	sw	zero,4(zero) # 4 <pos_freq_domains>
1c008e10:	002047b7          	lui	a5,0x204
1c008e14:	1007a423          	sw	zero,264(a5) # 204108 <__l1_heap_size+0x1c4128>
1c008e18:	40b2                	lw	ra,12(sp)
1c008e1a:	4422                	lw	s0,8(sp)
1c008e1c:	0141                	addi	sp,sp,16
1c008e1e:	8082                	ret

1c008e20 <exit>:
1c008e20:	1141                	addi	sp,sp,-16
1c008e22:	c422                	sw	s0,8(sp)
1c008e24:	c606                	sw	ra,12(sp)
1c008e26:	842a                	mv	s0,a0
1c008e28:	106020ef          	jal	ra,1c00af2e <pos_init_stop>
1c008e2c:	c1f44433          	p.bset	s0,s0,0,31
1c008e30:	1a1047b7          	lui	a5,0x1a104
1c008e34:	0a87a023          	sw	s0,160(a5) # 1a1040a0 <__l1_end+0xa104080>
1c008e38:	1a10a7b7          	lui	a5,0x1a10a
1c008e3c:	577d                	li	a4,-1
1c008e3e:	80e7a423          	sw	a4,-2040(a5) # 1a109808 <__l1_end+0xa1097e8>
1c008e42:	10500073          	wfi
1c008e46:	10500073          	wfi
1c008e4a:	bfe5                	j	1c008e42 <exit+0x22>

1c008e4c <pos_io_start>:
1c008e4c:	1101                	addi	sp,sp,-32
1c008e4e:	0048                	addi	a0,sp,4
1c008e50:	ce06                	sw	ra,28(sp)
1c008e52:	cc22                	sw	s0,24(sp)
1c008e54:	ca26                	sw	s1,20(sp)
1c008e56:	46a020ef          	jal	ra,1c00b2c0 <pi_uart_conf_init>
1c008e5a:	67f1                	lui	a5,0x1c
1c008e5c:	01000513          	li	a0,16
1c008e60:	20078793          	addi	a5,a5,512 # 1c200 <pos_soc_event_callback+0x1bdbc>
1c008e64:	004c                	addi	a1,sp,4
1c008e66:	4485                	li	s1,1
1c008e68:	00911623          	sh	s1,12(sp)
1c008e6c:	c23e                	sw	a5,4(sp)
1c008e6e:	102020ef          	jal	ra,1c00af70 <pi_open_from_conf>
1c008e72:	01000513          	li	a0,16
1c008e76:	462020ef          	jal	ra,1c00b2d8 <pi_uart_open>
1c008e7a:	e911                	bnez	a0,1c008e8e <pos_io_start+0x42>
1c008e7c:	1c0017b7          	lui	a5,0x1c001
1c008e80:	4097a823          	sw	s1,1040(a5) # 1c001410 <pos_io_uart_enabled>
1c008e84:	40f2                	lw	ra,28(sp)
1c008e86:	4462                	lw	s0,24(sp)
1c008e88:	44d2                	lw	s1,20(sp)
1c008e8a:	6105                	addi	sp,sp,32
1c008e8c:	8082                	ret
1c008e8e:	557d                	li	a0,-1
1c008e90:	bfd5                	j	1c008e84 <pos_io_start+0x38>

1c008e92 <pos_io_stop>:
1c008e92:	1141                	addi	sp,sp,-16
1c008e94:	01000513          	li	a0,16
1c008e98:	1c0017b7          	lui	a5,0x1c001
1c008e9c:	c606                	sw	ra,12(sp)
1c008e9e:	4007a823          	sw	zero,1040(a5) # 1c001410 <pos_io_uart_enabled>
1c008ea2:	55a020ef          	jal	ra,1c00b3fc <pi_uart_close>
1c008ea6:	40b2                	lw	ra,12(sp)
1c008ea8:	4501                	li	a0,0
1c008eaa:	0141                	addi	sp,sp,16
1c008eac:	8082                	ret

1c008eae <pos_io_init>:
1c008eae:	1c0017b7          	lui	a5,0x1c001
1c008eb2:	4007a823          	sw	zero,1040(a5) # 1c001410 <pos_io_uart_enabled>
1c008eb6:	8082                	ret

1c008eb8 <printf>:
1c008eb8:	7139                	addi	sp,sp,-64
1c008eba:	02410313          	addi	t1,sp,36
1c008ebe:	d432                	sw	a2,40(sp)
1c008ec0:	862a                	mv	a2,a0
1c008ec2:	1c009537          	lui	a0,0x1c009
1c008ec6:	d22e                	sw	a1,36(sp)
1c008ec8:	d636                	sw	a3,44(sp)
1c008eca:	4589                	li	a1,2
1c008ecc:	869a                	mv	a3,t1
1c008ece:	cd850513          	addi	a0,a0,-808 # 1c008cd8 <pos_libc_fputc_locked>
1c008ed2:	ce06                	sw	ra,28(sp)
1c008ed4:	d83a                	sw	a4,48(sp)
1c008ed6:	da3e                	sw	a5,52(sp)
1c008ed8:	dc42                	sw	a6,56(sp)
1c008eda:	de46                	sw	a7,60(sp)
1c008edc:	c61a                	sw	t1,12(sp)
1c008ede:	35c5                	jal	1c008dbe <pos_libc_prf_locked>
1c008ee0:	40f2                	lw	ra,28(sp)
1c008ee2:	6121                	addi	sp,sp,64
1c008ee4:	8082                	ret

1c008ee6 <pos_libc_prf>:
1c008ee6:	7175                	addi	sp,sp,-144
1c008ee8:	dad6                	sw	s5,116(sp)
1c008eea:	c706                	sw	ra,140(sp)
1c008eec:	c522                	sw	s0,136(sp)
1c008eee:	c326                	sw	s1,132(sp)
1c008ef0:	c14a                	sw	s2,128(sp)
1c008ef2:	dece                	sw	s3,124(sp)
1c008ef4:	dcd2                	sw	s4,120(sp)
1c008ef6:	d8da                	sw	s6,112(sp)
1c008ef8:	d6de                	sw	s7,108(sp)
1c008efa:	d4e2                	sw	s8,104(sp)
1c008efc:	d2e6                	sw	s9,100(sp)
1c008efe:	d0ea                	sw	s10,96(sp)
1c008f00:	ceee                	sw	s11,92(sp)
1c008f02:	8ab2                	mv	s5,a2
1c008f04:	001ac78b          	p.lbu	a5,1(s5!)
1c008f08:	cc36                	sw	a3,24(sp)
1c008f0a:	780786e3          	beqz	a5,1c009e96 <pos_libc_prf+0xfb0>
1c008f0e:	7761                	lui	a4,0xffff8
1c008f10:	83074713          	xori	a4,a4,-2000
1c008f14:	ce3a                	sw	a4,28(sp)
1c008f16:	1858                	addi	a4,sp,52
1c008f18:	fff74713          	not	a4,a4
1c008f1c:	d23a                	sw	a4,36(sp)
1c008f1e:	03610713          	addi	a4,sp,54
1c008f22:	fff74713          	not	a4,a4
1c008f26:	8a2a                	mv	s4,a0
1c008f28:	84ae                	mv	s1,a1
1c008f2a:	4b01                	li	s6,0
1c008f2c:	d03a                	sw	a4,32(sp)
1c008f2e:	8d56                	mv	s10,s5
1c008f30:	853e                	mv	a0,a5
1c008f32:	a819                	j	1c008f48 <pos_libc_prf+0x62>
1c008f34:	85a6                	mv	a1,s1
1c008f36:	9a02                	jalr	s4
1c008f38:	19f52763          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c008f3c:	0b05                	addi	s6,s6,1
1c008f3e:	8cea                	mv	s9,s10
1c008f40:	8d66                	mv	s10,s9
1c008f42:	001d450b          	p.lbu	a0,1(s10!)
1c008f46:	cd29                	beqz	a0,1c008fa0 <pos_libc_prf+0xba>
1c008f48:	02500793          	li	a5,37
1c008f4c:	fef514e3          	bne	a0,a5,1c008f34 <pos_libc_prf+0x4e>
1c008f50:	4c01                	li	s8,0
1c008f52:	c802                	sw	zero,16(sp)
1c008f54:	ca02                	sw	zero,20(sp)
1c008f56:	c602                	sw	zero,12(sp)
1c008f58:	4901                	li	s2,0
1c008f5a:	02b00413          	li	s0,43
1c008f5e:	02d00a93          	li	s5,45
1c008f62:	03000d93          	li	s11,48
1c008f66:	02000993          	li	s3,32
1c008f6a:	000d4b83          	lbu	s7,0(s10)
1c008f6e:	1c0017b7          	lui	a5,0x1c001
1c008f72:	bc478513          	addi	a0,a5,-1084 # 1c000bc4 <__clz_tab+0x35c>
1c008f76:	85de                	mv	a1,s7
1c008f78:	3b35                	jal	1c008cb4 <strchr>
1c008f7a:	001d0c93          	addi	s9,s10,1
1c008f7e:	86de                	mv	a3,s7
1c008f80:	c13d                	beqz	a0,1c008fe6 <pos_libc_prf+0x100>
1c008f82:	048b8e63          	beq	s7,s0,1c008fde <pos_libc_prf+0xf8>
1c008f86:	03746d63          	bltu	s0,s7,1c008fc0 <pos_libc_prf+0xda>
1c008f8a:	053b8663          	beq	s7,s3,1c008fd6 <pos_libc_prf+0xf0>
1c008f8e:	02300793          	li	a5,35
1c008f92:	00fb9563          	bne	s7,a5,1c008f9c <pos_libc_prf+0xb6>
1c008f96:	4905                	li	s2,1
1c008f98:	8d66                	mv	s10,s9
1c008f9a:	bfc1                	j	1c008f6a <pos_libc_prf+0x84>
1c008f9c:	fe0b9ee3          	bnez	s7,1c008f98 <pos_libc_prf+0xb2>
1c008fa0:	40ba                	lw	ra,140(sp)
1c008fa2:	442a                	lw	s0,136(sp)
1c008fa4:	449a                	lw	s1,132(sp)
1c008fa6:	490a                	lw	s2,128(sp)
1c008fa8:	59f6                	lw	s3,124(sp)
1c008faa:	5a66                	lw	s4,120(sp)
1c008fac:	5ad6                	lw	s5,116(sp)
1c008fae:	5bb6                	lw	s7,108(sp)
1c008fb0:	5c26                	lw	s8,104(sp)
1c008fb2:	5c96                	lw	s9,100(sp)
1c008fb4:	5d06                	lw	s10,96(sp)
1c008fb6:	4df6                	lw	s11,92(sp)
1c008fb8:	855a                	mv	a0,s6
1c008fba:	5b46                	lw	s6,112(sp)
1c008fbc:	6149                	addi	sp,sp,144
1c008fbe:	8082                	ret
1c008fc0:	015b8763          	beq	s7,s5,1c008fce <pos_libc_prf+0xe8>
1c008fc4:	fdbb9ae3          	bne	s7,s11,1c008f98 <pos_libc_prf+0xb2>
1c008fc8:	4c05                	li	s8,1
1c008fca:	8d66                	mv	s10,s9
1c008fcc:	bf79                	j	1c008f6a <pos_libc_prf+0x84>
1c008fce:	4785                	li	a5,1
1c008fd0:	c63e                	sw	a5,12(sp)
1c008fd2:	8d66                	mv	s10,s9
1c008fd4:	bf59                	j	1c008f6a <pos_libc_prf+0x84>
1c008fd6:	4785                	li	a5,1
1c008fd8:	c83e                	sw	a5,16(sp)
1c008fda:	8d66                	mv	s10,s9
1c008fdc:	b779                	j	1c008f6a <pos_libc_prf+0x84>
1c008fde:	4785                	li	a5,1
1c008fe0:	ca3e                	sw	a5,20(sp)
1c008fe2:	8d66                	mv	s10,s9
1c008fe4:	b759                	j	1c008f6a <pos_libc_prf+0x84>
1c008fe6:	d64a                	sw	s2,44(sp)
1c008fe8:	02a00793          	li	a5,42
1c008fec:	8966                	mv	s2,s9
1c008fee:	10fb8263          	beq	s7,a5,1c0090f2 <pos_libc_prf+0x20c>
1c008ff2:	fd0b8793          	addi	a5,s7,-48
1c008ff6:	4525                	li	a0,9
1c008ff8:	4d81                	li	s11,0
1c008ffa:	0cf57963          	bgeu	a0,a5,1c0090cc <pos_libc_prf+0x1e6>
1c008ffe:	02e00793          	li	a5,46
1c009002:	54f68a63          	beq	a3,a5,1c009556 <pos_libc_prf+0x670>
1c009006:	89b6                	mv	s3,a3
1c009008:	547d                	li	s0,-1
1c00900a:	1c0017b7          	lui	a5,0x1c001
1c00900e:	85b6                	mv	a1,a3
1c009010:	bcc78513          	addi	a0,a5,-1076 # 1c000bcc <__clz_tab+0x364>
1c009014:	d436                	sw	a3,40(sp)
1c009016:	3979                	jal	1c008cb4 <strchr>
1c009018:	56a2                	lw	a3,40(sp)
1c00901a:	4601                	li	a2,0
1c00901c:	cd11                	beqz	a0,1c009038 <pos_libc_prf+0x152>
1c00901e:	87e6                	mv	a5,s9
1c009020:	06c00713          	li	a4,108
1c009024:	0017c68b          	p.lbu	a3,1(a5!)
1c009028:	54e98763          	beq	s3,a4,1c009576 <pos_libc_prf+0x690>
1c00902c:	06800713          	li	a4,104
1c009030:	54e98963          	beq	s3,a4,1c009582 <pos_libc_prf+0x69c>
1c009034:	8cbe                	mv	s9,a5
1c009036:	864e                	mv	a2,s3
1c009038:	06700793          	li	a5,103
1c00903c:	2ed7e963          	bltu	a5,a3,1c00932e <pos_libc_prf+0x448>
1c009040:	06400793          	li	a5,100
1c009044:	3ad7ede3          	bltu	a5,a3,1c009bfe <pos_libc_prf+0xd18>
1c009048:	04700713          	li	a4,71
1c00904c:	0cd76263          	bltu	a4,a3,1c009110 <pos_libc_prf+0x22a>
1c009050:	04400793          	li	a5,68
1c009054:	3ad7e5e3          	bltu	a5,a3,1c009bfe <pos_libc_prf+0xd18>
1c009058:	d6a1                	beqz	a3,1c008fa0 <pos_libc_prf+0xba>
1c00905a:	02500793          	li	a5,37
1c00905e:	46f69d63          	bne	a3,a5,1c0094d8 <pos_libc_prf+0x5f2>
1c009062:	85a6                	mv	a1,s1
1c009064:	02500513          	li	a0,37
1c009068:	9a02                	jalr	s4
1c00906a:	05f52e63          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c00906e:	0b05                	addi	s6,s6,1
1c009070:	bdc1                	j	1c008f40 <pos_libc_prf+0x5a>
1c009072:	5722                	lw	a4,40(sp)
1c009074:	8bbe                	mv	s7,a5
1c009076:	fff70c13          	addi	s8,a4,-1 # ffff7fff <__l2_shared_end+0xe3fe7bf7>
1c00907a:	85a6                	mv	a1,s1
1c00907c:	03000513          	li	a0,48
1c009080:	9a02                	jalr	s4
1c009082:	1c7d                	addi	s8,s8,-1
1c009084:	05f52163          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c009088:	fffc39e3          	p.bneimm	s8,-1,1c00907a <pos_libc_prf+0x194>
1c00908c:	5722                	lw	a4,40(sp)
1c00908e:	40eb8c33          	sub	s8,s7,a4
1c009092:	640401e3          	beqz	s0,1c009ed4 <pos_libc_prf+0xfee>
1c009096:	000ac703          	lbu	a4,0(s5)
1c00909a:	8dd6                	mv	s11,s5
1c00909c:	9c56                	add	s8,s8,s5
1c00909e:	02e00b93          	li	s7,46
1c0090a2:	4aa5                	li	s5,9
1c0090a4:	a019                	j	1c0090aa <pos_libc_prf+0x1c4>
1c0090a6:	000dc703          	lbu	a4,0(s11)
1c0090aa:	fd070793          	addi	a5,a4,-48
1c0090ae:	853a                	mv	a0,a4
1c0090b0:	85a6                	mv	a1,s1
1c0090b2:	41bc06b3          	sub	a3,s8,s11
1c0090b6:	00faf463          	bgeu	s5,a5,1c0090be <pos_libc_prf+0x1d8>
1c0090ba:	5f771ee3          	bne	a4,s7,1c009eb6 <pos_libc_prf+0xfd0>
1c0090be:	0d85                	addi	s11,s11,1
1c0090c0:	9a02                	jalr	s4
1c0090c2:	fff532e3          	p.bneimm	a0,-1,1c0090a6 <pos_libc_prf+0x1c0>
1c0090c6:	5b7d                	li	s6,-1
1c0090c8:	bde1                	j	1c008fa0 <pos_libc_prf+0xba>
1c0090ca:	0905                	addi	s2,s2,1
1c0090cc:	00094683          	lbu	a3,0(s2)
1c0090d0:	002d9793          	slli	a5,s11,0x2
1c0090d4:	97ee                	add	a5,a5,s11
1c0090d6:	0786                	slli	a5,a5,0x1
1c0090d8:	97de                	add	a5,a5,s7
1c0090da:	fd068713          	addi	a4,a3,-48
1c0090de:	886a                	mv	a6,s10
1c0090e0:	fd078d93          	addi	s11,a5,-48
1c0090e4:	8d4a                	mv	s10,s2
1c0090e6:	8bb6                	mv	s7,a3
1c0090e8:	fee571e3          	bgeu	a0,a4,1c0090ca <pos_libc_prf+0x1e4>
1c0090ec:	00280c93          	addi	s9,a6,2
1c0090f0:	b739                	j	1c008ffe <pos_libc_prf+0x118>
1c0090f2:	47e2                	lw	a5,24(sp)
1c0090f4:	0047ad8b          	p.lw	s11,4(a5!)
1c0090f8:	000dd663          	bgez	s11,1c009104 <pos_libc_prf+0x21e>
1c0090fc:	4705                	li	a4,1
1c0090fe:	41b00db3          	neg	s11,s11
1c009102:	c63a                	sw	a4,12(sp)
1c009104:	00094683          	lbu	a3,0(s2)
1c009108:	cc3e                	sw	a5,24(sp)
1c00910a:	002d0c93          	addi	s9,s10,2
1c00910e:	bdc5                	j	1c008ffe <pos_libc_prf+0x118>
1c009110:	06300713          	li	a4,99
1c009114:	0ae69463          	bne	a3,a4,1c0091bc <pos_libc_prf+0x2d6>
1c009118:	4d62                	lw	s10,24(sp)
1c00911a:	fffd8913          	addi	s2,s11,-1
1c00911e:	ca02                	sw	zero,20(sp)
1c009120:	004d278b          	p.lw	a5,4(s10!)
1c009124:	c802                	sw	zero,16(sp)
1c009126:	4985                	li	s3,1
1c009128:	02f10a23          	sb	a5,52(sp)
1c00912c:	03410c13          	addi	s8,sp,52
1c009130:	4401                	li	s0,0
1c009132:	d402                	sw	zero,40(sp)
1c009134:	cc02                	sw	zero,24(sp)
1c009136:	47b2                	lw	a5,12(sp)
1c009138:	e385                	bnez	a5,1c009158 <pos_libc_prf+0x272>
1c00913a:	01205f63          	blez	s2,1c009158 <pos_libc_prf+0x272>
1c00913e:	fff90a93          	addi	s5,s2,-1
1c009142:	85a6                	mv	a1,s1
1c009144:	02000513          	li	a0,32
1c009148:	9a02                	jalr	s4
1c00914a:	1afd                	addi	s5,s5,-1
1c00914c:	f7f52de3          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c009150:	fffab9e3          	p.bneimm	s5,-1,1c009142 <pos_libc_prf+0x25c>
1c009154:	9b4a                	add	s6,s6,s2
1c009156:	597d                	li	s2,-1
1c009158:	47c2                	lw	a5,16(sp)
1c00915a:	00fc0ab3          	add	s5,s8,a5
1c00915e:	a031                	j	1c00916a <pos_libc_prf+0x284>
1c009160:	001c450b          	p.lbu	a0,1(s8!)
1c009164:	9a02                	jalr	s4
1c009166:	f7f520e3          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c00916a:	85a6                	mv	a1,s1
1c00916c:	ff5c1ae3          	bne	s8,s5,1c009160 <pos_libc_prf+0x27a>
1c009170:	4bd2                	lw	s7,20(sp)
1c009172:	a021                	j	1c00917a <pos_libc_prf+0x294>
1c009174:	9a02                	jalr	s4
1c009176:	f5f528e3          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c00917a:	1bfd                	addi	s7,s7,-1
1c00917c:	85a6                	mv	a1,s1
1c00917e:	03000513          	li	a0,48
1c009182:	fffbb9e3          	p.bneimm	s7,-1,1c009174 <pos_libc_prf+0x28e>
1c009186:	47c2                	lw	a5,16(sp)
1c009188:	40f989b3          	sub	s3,s3,a5
1c00918c:	47e2                	lw	a5,24(sp)
1c00918e:	8c4e                	mv	s8,s3
1c009190:	640792e3          	bnez	a5,1c009fd4 <pos_libc_prf+0x10ee>
1c009194:	57a2                	lw	a5,40(sp)
1c009196:	ee078ee3          	beqz	a5,1c009092 <pos_libc_prf+0x1ac>
1c00919a:	9c56                	add	s8,s8,s5
1c00919c:	02e00d93          	li	s11,46
1c0091a0:	a029                	j	1c0091aa <pos_libc_prf+0x2c4>
1c0091a2:	415c07b3          	sub	a5,s8,s5
1c0091a6:	edbb86e3          	beq	s7,s11,1c009072 <pos_libc_prf+0x18c>
1c0091aa:	001acb8b          	p.lbu	s7,1(s5!)
1c0091ae:	85a6                	mv	a1,s1
1c0091b0:	855e                	mv	a0,s7
1c0091b2:	9a02                	jalr	s4
1c0091b4:	fff537e3          	p.bneimm	a0,-1,1c0091a2 <pos_libc_prf+0x2bc>
1c0091b8:	5b7d                	li	s6,-1
1c0091ba:	b3dd                	j	1c008fa0 <pos_libc_prf+0xba>
1c0091bc:	1af69b63          	bne	a3,a5,1c009372 <pos_libc_prf+0x48c>
1c0091c0:	06c00793          	li	a5,108
1c0091c4:	00f60c63          	beq	a2,a5,1c0091dc <pos_libc_prf+0x2f6>
1c0091c8:	07a00793          	li	a5,122
1c0091cc:	00f60863          	beq	a2,a5,1c0091dc <pos_libc_prf+0x2f6>
1c0091d0:	04c00793          	li	a5,76
1c0091d4:	00f61463          	bne	a2,a5,1c0091dc <pos_libc_prf+0x2f6>
1c0091d8:	78c0106f          	j	1c00a964 <pos_libc_prf+0x1a7e>
1c0091dc:	4d62                	lw	s10,24(sp)
1c0091de:	004d278b          	p.lw	a5,4(s10!)
1c0091e2:	41f7d713          	srai	a4,a5,0x1f
1c0091e6:	d43a                	sw	a4,40(sp)
1c0091e8:	86ba                	mv	a3,a4
1c0091ea:	5a06cce3          	bltz	a3,1c009fa2 <pos_libc_prf+0x10bc>
1c0091ee:	4752                	lw	a4,20(sp)
1c0091f0:	64070ae3          	beqz	a4,1c00a044 <pos_libc_prf+0x115e>
1c0091f4:	02b00713          	li	a4,43
1c0091f8:	02e10a23          	sb	a4,52(sp)
1c0091fc:	03510713          	addi	a4,sp,53
1c009200:	89b6                	mv	s3,a3
1c009202:	cc3a                	sw	a4,24(sp)
1c009204:	893a                	mv	s2,a4
1c009206:	8abe                	mv	s5,a5
1c009208:	4629                	li	a2,10
1c00920a:	4681                	li	a3,0
1c00920c:	8556                	mv	a0,s5
1c00920e:	85ce                	mv	a1,s3
1c009210:	9ccff0ef          	jal	ra,1c0083dc <__umoddi3>
1c009214:	8baa                	mv	s7,a0
1c009216:	85ce                	mv	a1,s3
1c009218:	8556                	mv	a0,s5
1c00921a:	4629                	li	a2,10
1c00921c:	4681                	li	a3,0
1c00921e:	e7ffe0ef          	jal	ra,1c00809c <__udivdi3>
1c009222:	030b8b93          	addi	s7,s7,48
1c009226:	87ce                	mv	a5,s3
1c009228:	864a                	mv	a2,s2
1c00922a:	017900ab          	p.sb	s7,1(s2!)
1c00922e:	8756                	mv	a4,s5
1c009230:	89ae                	mv	s3,a1
1c009232:	8aaa                	mv	s5,a0
1c009234:	fbf1                	bnez	a5,1c009208 <pos_libc_prf+0x322>
1c009236:	47a5                	li	a5,9
1c009238:	fce7e8e3          	bltu	a5,a4,1c009208 <pos_libc_prf+0x322>
1c00923c:	47e2                	lw	a5,24(sp)
1c00923e:	00090023          	sb	zero,0(s2)
1c009242:	0ac7f863          	bgeu	a5,a2,1c0092f2 <pos_libc_prf+0x40c>
1c009246:	fff7c693          	not	a3,a5
1c00924a:	96b2                	add	a3,a3,a2
1c00924c:	883e                	mv	a6,a5
1c00924e:	0016d793          	srli	a5,a3,0x1
1c009252:	00178e13          	addi	t3,a5,1
1c009256:	01c80733          	add	a4,a6,t3
1c00925a:	40f607b3          	sub	a5,a2,a5
1c00925e:	04f737b3          	p.sletu	a5,a4,a5
1c009262:	05093733          	p.sletu	a4,s2,a6
1c009266:	8fd9                	or	a5,a5,a4
1c009268:	85c2                	mv	a1,a6
1c00926a:	8732                	mv	a4,a2
1c00926c:	e399                	bnez	a5,1c009272 <pos_libc_prf+0x38c>
1c00926e:	1ef0106f          	j	1c00ac5c <pos_libc_prf+0x1d76>
1c009272:	0066b693          	sltiu	a3,a3,6
1c009276:	c299                	beqz	a3,1c00927c <pos_libc_prf+0x396>
1c009278:	1e50106f          	j	1c00ac5c <pos_libc_prf+0x1d76>
1c00927c:	c20e3533          	p.bclr	a0,t3,1,0
1c009280:	ffd60693          	addi	a3,a2,-3 # fffffd <__l1_heap_size+0xfc001d>
1c009284:	9542                	add	a0,a0,a6
1c009286:	4198                	lw	a4,0(a1)
1c009288:	429c                	lw	a5,0(a3)
1c00928a:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c00928e:	fee6ae2b          	p.sw	a4,-4(a3!)
1c009292:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c009296:	00f5a22b          	p.sw	a5,4(a1!)
1c00929a:	fea596e3          	bne	a1,a0,1c009286 <pos_libc_prf+0x3a0>
1c00929e:	4762                	lw	a4,24(sp)
1c0092a0:	c20e37b3          	p.bclr	a5,t3,1,0
1c0092a4:	8e1d                	sub	a2,a2,a5
1c0092a6:	00f70ab3          	add	s5,a4,a5
1c0092aa:	05c78463          	beq	a5,t3,1c0092f2 <pos_libc_prf+0x40c>
1c0092ae:	000ac703          	lbu	a4,0(s5)
1c0092b2:	00064683          	lbu	a3,0(a2)
1c0092b6:	87b2                	mv	a5,a2
1c0092b8:	fee78fab          	p.sb	a4,-1(a5!)
1c0092bc:	8756                	mv	a4,s5
1c0092be:	00d700ab          	p.sb	a3,1(a4!)
1c0092c2:	02f77863          	bgeu	a4,a5,1c0092f2 <pos_libc_prf+0x40c>
1c0092c6:	001ac683          	lbu	a3,1(s5)
1c0092ca:	fff64703          	lbu	a4,-1(a2)
1c0092ce:	002a8793          	addi	a5,s5,2
1c0092d2:	fed60fa3          	sb	a3,-1(a2)
1c0092d6:	00ea80a3          	sb	a4,1(s5)
1c0092da:	ffe60713          	addi	a4,a2,-2
1c0092de:	00e7fa63          	bgeu	a5,a4,1c0092f2 <pos_libc_prf+0x40c>
1c0092e2:	002ac703          	lbu	a4,2(s5)
1c0092e6:	ffe64783          	lbu	a5,-2(a2)
1c0092ea:	fee60f23          	sb	a4,-2(a2)
1c0092ee:	00fa8123          	sb	a5,2(s5)
1c0092f2:	185c                	addi	a5,sp,52
1c0092f4:	40f909b3          	sub	s3,s2,a5
1c0092f8:	47d2                	lw	a5,20(sp)
1c0092fa:	3a079ae3          	bnez	a5,1c009eae <pos_libc_prf+0xfc8>
1c0092fe:	47c2                	lw	a5,16(sp)
1c009300:	3a0797e3          	bnez	a5,1c009eae <pos_libc_prf+0xfc8>
1c009304:	57a2                	lw	a5,40(sp)
1c009306:	83fd                	srli	a5,a5,0x1f
1c009308:	c83e                	sw	a5,16(sp)
1c00930a:	0c045ce3          	bgez	s0,1c009be2 <pos_libc_prf+0xcfc>
1c00930e:	413d8933          	sub	s2,s11,s3
1c009312:	4401                	li	s0,0
1c009314:	d402                	sw	zero,40(sp)
1c009316:	cc02                	sw	zero,24(sp)
1c009318:	0c0c00e3          	beqz	s8,1c009bd8 <pos_libc_prf+0xcf2>
1c00931c:	040967b3          	p.max	a5,s2,zero
1c009320:	ca3e                	sw	a5,20(sp)
1c009322:	97ce                	add	a5,a5,s3
1c009324:	40fd8933          	sub	s2,s11,a5
1c009328:	03410c13          	addi	s8,sp,52
1c00932c:	b529                	j	1c009136 <pos_libc_prf+0x250>
1c00932e:	07000793          	li	a5,112
1c009332:	24f683e3          	beq	a3,a5,1c009d78 <pos_libc_prf+0xe92>
1c009336:	1cd7f063          	bgeu	a5,a3,1c0094f6 <pos_libc_prf+0x610>
1c00933a:	07300793          	li	a5,115
1c00933e:	18f69963          	bne	a3,a5,1c0094d0 <pos_libc_prf+0x5ea>
1c009342:	4d62                	lw	s10,24(sp)
1c009344:	004d2c0b          	p.lw	s8,4(s10!)
1c009348:	24044f63          	bltz	s0,1c0095a6 <pos_libc_prf+0x6c0>
1c00934c:	e019                	bnez	s0,1c009352 <pos_libc_prf+0x46c>
1c00934e:	1610106f          	j	1c00acae <pos_libc_prf+0x1dc8>
1c009352:	4705                	li	a4,1
1c009354:	87e2                	mv	a5,s8
1c009356:	4981                	li	s3,0
1c009358:	04e46433          	p.max	s0,s0,a4
1c00935c:	005440fb          	lp.setup	x1,s0,1c009366 <pos_libc_prf+0x480>
1c009360:	0017c70b          	p.lbu	a4,1(a5!)
1c009364:	c311                	beqz	a4,1c009368 <pos_libc_prf+0x482>
1c009366:	0985                	addi	s3,s3,1
1c009368:	413d8933          	sub	s2,s11,s3
1c00936c:	ca02                	sw	zero,20(sp)
1c00936e:	c802                	sw	zero,16(sp)
1c009370:	b3c1                	j	1c009130 <pos_libc_prf+0x24a>
1c009372:	05800793          	li	a5,88
1c009376:	16f69163          	bne	a3,a5,1c0094d8 <pos_libc_prf+0x5f2>
1c00937a:	06c00793          	li	a5,108
1c00937e:	00f60c63          	beq	a2,a5,1c009396 <pos_libc_prf+0x4b0>
1c009382:	07a00793          	li	a5,122
1c009386:	00f60863          	beq	a2,a5,1c009396 <pos_libc_prf+0x4b0>
1c00938a:	04c00793          	li	a5,76
1c00938e:	00f61463          	bne	a2,a5,1c009396 <pos_libc_prf+0x4b0>
1c009392:	5ec0106f          	j	1c00a97e <pos_libc_prf+0x1a98>
1c009396:	4d62                	lw	s10,24(sp)
1c009398:	4a81                	li	s5,0
1c00939a:	004d280b          	p.lw	a6,4(s10!)
1c00939e:	06f00793          	li	a5,111
1c0093a2:	00f69463          	bne	a3,a5,1c0093aa <pos_libc_prf+0x4c4>
1c0093a6:	7800106f          	j	1c00ab26 <pos_libc_prf+0x1c40>
1c0093aa:	07500793          	li	a5,117
1c0093ae:	00f69463          	bne	a3,a5,1c0093b6 <pos_libc_prf+0x4d0>
1c0093b2:	5ea0106f          	j	1c00a99c <pos_libc_prf+0x1ab6>
1c0093b6:	57b2                	lw	a5,44(sp)
1c0093b8:	1848                	addi	a0,sp,52
1c0093ba:	c791                	beqz	a5,1c0093c6 <pos_libc_prf+0x4e0>
1c0093bc:	47f2                	lw	a5,28(sp)
1c0093be:	03610513          	addi	a0,sp,54
1c0093c2:	02f11a23          	sh	a5,52(sp)
1c0093c6:	85aa                	mv	a1,a0
1c0093c8:	4ea5                	li	t4,9
1c0093ca:	4f3d                	li	t5,15
1c0093cc:	a801                	j	1c0093dc <pos_libc_prf+0x4f6>
1c0093ce:	006980ab          	p.sb	t1,1(s3!)
1c0093d2:	020a8963          	beqz	s5,1c009404 <pos_libc_prf+0x51e>
1c0093d6:	8832                	mv	a6,a2
1c0093d8:	8aba                	mv	s5,a4
1c0093da:	85ce                	mv	a1,s3
1c0093dc:	f64837b3          	p.bclr	a5,a6,27,4
1c0093e0:	01ca9713          	slli	a4,s5,0x1c
1c0093e4:	00485613          	srli	a2,a6,0x4
1c0093e8:	8e59                	or	a2,a2,a4
1c0093ea:	89ae                	mv	s3,a1
1c0093ec:	03078313          	addi	t1,a5,48
1c0093f0:	05778893          	addi	a7,a5,87
1c0093f4:	004ad713          	srli	a4,s5,0x4
1c0093f8:	fcfefbe3          	bgeu	t4,a5,1c0093ce <pos_libc_prf+0x4e8>
1c0093fc:	011980ab          	p.sb	a7,1(s3!)
1c009400:	fc0a9be3          	bnez	s5,1c0093d6 <pos_libc_prf+0x4f0>
1c009404:	fd0f69e3          	bltu	t5,a6,1c0093d6 <pos_libc_prf+0x4f0>
1c009408:	00098023          	sb	zero,0(s3)
1c00940c:	0ab57563          	bgeu	a0,a1,1c0094b6 <pos_libc_prf+0x5d0>
1c009410:	fff54313          	not	t1,a0
1c009414:	932e                	add	t1,t1,a1
1c009416:	00135793          	srli	a5,t1,0x1
1c00941a:	00178e13          	addi	t3,a5,1
1c00941e:	40f58733          	sub	a4,a1,a5
1c009422:	01c507b3          	add	a5,a0,t3
1c009426:	04e7b733          	p.sletu	a4,a5,a4
1c00942a:	04a9b7b3          	p.sletu	a5,s3,a0
1c00942e:	8f5d                	or	a4,a4,a5
1c009430:	862a                	mv	a2,a0
1c009432:	87ae                	mv	a5,a1
1c009434:	16070e63          	beqz	a4,1c0095b0 <pos_libc_prf+0x6ca>
1c009438:	00633313          	sltiu	t1,t1,6
1c00943c:	16031a63          	bnez	t1,1c0095b0 <pos_libc_prf+0x6ca>
1c009440:	c20e3eb3          	p.bclr	t4,t3,1,0
1c009444:	ffd58313          	addi	t1,a1,-3
1c009448:	9eaa                	add	t4,t4,a0
1c00944a:	4218                	lw	a4,0(a2)
1c00944c:	00032783          	lw	a5,0(t1)
1c009450:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c009454:	fee32e2b          	p.sw	a4,-4(t1!)
1c009458:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c00945c:	00f6222b          	p.sw	a5,4(a2!)
1c009460:	fece95e3          	bne	t4,a2,1c00944a <pos_libc_prf+0x564>
1c009464:	c20e3733          	p.bclr	a4,t3,1,0
1c009468:	8d99                	sub	a1,a1,a4
1c00946a:	00e507b3          	add	a5,a0,a4
1c00946e:	05c70463          	beq	a4,t3,1c0094b6 <pos_libc_prf+0x5d0>
1c009472:	0007c603          	lbu	a2,0(a5)
1c009476:	0005c503          	lbu	a0,0(a1)
1c00947a:	872e                	mv	a4,a1
1c00947c:	fec70fab          	p.sb	a2,-1(a4!)
1c009480:	863e                	mv	a2,a5
1c009482:	00a600ab          	p.sb	a0,1(a2!)
1c009486:	02e67863          	bgeu	a2,a4,1c0094b6 <pos_libc_prf+0x5d0>
1c00948a:	0017c503          	lbu	a0,1(a5)
1c00948e:	fff5c603          	lbu	a2,-1(a1)
1c009492:	00278713          	addi	a4,a5,2
1c009496:	fea58fa3          	sb	a0,-1(a1)
1c00949a:	00c780a3          	sb	a2,1(a5)
1c00949e:	ffe58613          	addi	a2,a1,-2
1c0094a2:	00c77a63          	bgeu	a4,a2,1c0094b6 <pos_libc_prf+0x5d0>
1c0094a6:	0027c603          	lbu	a2,2(a5)
1c0094aa:	ffe5c703          	lbu	a4,-2(a1)
1c0094ae:	fec58f23          	sb	a2,-2(a1)
1c0094b2:	00e78123          	sb	a4,2(a5)
1c0094b6:	05800793          	li	a5,88
1c0094ba:	00f69463          	bne	a3,a5,1c0094c2 <pos_libc_prf+0x5dc>
1c0094be:	61c0106f          	j	1c00aada <pos_libc_prf+0x1bf4>
1c0094c2:	185c                	addi	a5,sp,52
1c0094c4:	40f989b3          	sub	s3,s3,a5
1c0094c8:	57b2                	lw	a5,44(sp)
1c0094ca:	0786                	slli	a5,a5,0x1
1c0094cc:	c83e                	sw	a5,16(sp)
1c0094ce:	bd35                	j	1c00930a <pos_libc_prf+0x424>
1c0094d0:	07200793          	li	a5,114
1c0094d4:	04d7ef63          	bltu	a5,a3,1c009532 <pos_libc_prf+0x64c>
1c0094d8:	85a6                	mv	a1,s1
1c0094da:	02500513          	li	a0,37
1c0094de:	c636                	sw	a3,12(sp)
1c0094e0:	9a02                	jalr	s4
1c0094e2:	bff522e3          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c0094e6:	46b2                	lw	a3,12(sp)
1c0094e8:	85a6                	mv	a1,s1
1c0094ea:	8536                	mv	a0,a3
1c0094ec:	9a02                	jalr	s4
1c0094ee:	bdf52ce3          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c0094f2:	0b09                	addi	s6,s6,2
1c0094f4:	b4b1                	j	1c008f40 <pos_libc_prf+0x5a>
1c0094f6:	06e00793          	li	a5,110
1c0094fa:	04f69563          	bne	a3,a5,1c009544 <pos_libc_prf+0x65e>
1c0094fe:	46e2                	lw	a3,24(sp)
1c009500:	06800793          	li	a5,104
1c009504:	0046a70b          	p.lw	a4,4(a3!)
1c009508:	cc36                	sw	a3,24(sp)
1c00950a:	0af60e63          	beq	a2,a5,1c0095c6 <pos_libc_prf+0x6e0>
1c00950e:	18c7cce3          	blt	a5,a2,1c009ea6 <pos_libc_prf+0xfc0>
1c009512:	04800793          	li	a5,72
1c009516:	00f61463          	bne	a2,a5,1c00951e <pos_libc_prf+0x638>
1c00951a:	73a0106f          	j	1c00ac54 <pos_libc_prf+0x1d6e>
1c00951e:	04c00793          	li	a5,76
1c009522:	18f612e3          	bne	a2,a5,1c009ea6 <pos_libc_prf+0xfc0>
1c009526:	41fb5793          	srai	a5,s6,0x1f
1c00952a:	01672023          	sw	s6,0(a4)
1c00952e:	c35c                	sw	a5,4(a4)
1c009530:	bc01                	j	1c008f40 <pos_libc_prf+0x5a>
1c009532:	07500793          	li	a5,117
1c009536:	e4f682e3          	beq	a3,a5,1c00937a <pos_libc_prf+0x494>
1c00953a:	07800793          	li	a5,120
1c00953e:	e2f68ee3          	beq	a3,a5,1c00937a <pos_libc_prf+0x494>
1c009542:	bf59                	j	1c0094d8 <pos_libc_prf+0x5f2>
1c009544:	06f00793          	li	a5,111
1c009548:	e2f689e3          	beq	a3,a5,1c00937a <pos_libc_prf+0x494>
1c00954c:	06900793          	li	a5,105
1c009550:	c6f688e3          	beq	a3,a5,1c0091c0 <pos_libc_prf+0x2da>
1c009554:	b751                	j	1c0094d8 <pos_libc_prf+0x5f2>
1c009556:	87e6                	mv	a5,s9
1c009558:	0017c68b          	p.lbu	a3,1(a5!)
1c00955c:	02a00713          	li	a4,42
1c009560:	06e69663          	bne	a3,a4,1c0095cc <pos_libc_prf+0x6e6>
1c009564:	47e2                	lw	a5,24(sp)
1c009566:	001cc683          	lbu	a3,1(s9)
1c00956a:	0c89                	addi	s9,s9,2
1c00956c:	0047a40b          	p.lw	s0,4(a5!)
1c009570:	89b6                	mv	s3,a3
1c009572:	cc3e                	sw	a5,24(sp)
1c009574:	bc59                	j	1c00900a <pos_libc_prf+0x124>
1c009576:	01368c63          	beq	a3,s3,1c00958e <pos_libc_prf+0x6a8>
1c00957a:	8cbe                	mv	s9,a5
1c00957c:	06c00613          	li	a2,108
1c009580:	bc65                	j	1c009038 <pos_libc_prf+0x152>
1c009582:	01368c63          	beq	a3,s3,1c00959a <pos_libc_prf+0x6b4>
1c009586:	8cbe                	mv	s9,a5
1c009588:	06800613          	li	a2,104
1c00958c:	b475                	j	1c009038 <pos_libc_prf+0x152>
1c00958e:	001cc683          	lbu	a3,1(s9)
1c009592:	04c00613          	li	a2,76
1c009596:	0c89                	addi	s9,s9,2
1c009598:	b445                	j	1c009038 <pos_libc_prf+0x152>
1c00959a:	001cc683          	lbu	a3,1(s9)
1c00959e:	04800613          	li	a2,72
1c0095a2:	0c89                	addi	s9,s9,2
1c0095a4:	bc51                	j	1c009038 <pos_libc_prf+0x152>
1c0095a6:	80000437          	lui	s0,0x80000
1c0095aa:	fff44413          	not	s0,s0
1c0095ae:	b355                	j	1c009352 <pos_libc_prf+0x46c>
1c0095b0:	00054603          	lbu	a2,0(a0)
1c0095b4:	0007c703          	lbu	a4,0(a5)
1c0095b8:	fec78fab          	p.sb	a2,-1(a5!)
1c0095bc:	00e500ab          	p.sb	a4,1(a0!)
1c0095c0:	fef568e3          	bltu	a0,a5,1c0095b0 <pos_libc_prf+0x6ca>
1c0095c4:	bdcd                	j	1c0094b6 <pos_libc_prf+0x5d0>
1c0095c6:	01671023          	sh	s6,0(a4)
1c0095ca:	ba9d                	j	1c008f40 <pos_libc_prf+0x5a>
1c0095cc:	fd068593          	addi	a1,a3,-48
1c0095d0:	4525                	li	a0,9
1c0095d2:	89b6                	mv	s3,a3
1c0095d4:	873e                	mv	a4,a5
1c0095d6:	4401                	li	s0,0
1c0095d8:	4625                	li	a2,9
1c0095da:	8cbe                	mv	s9,a5
1c0095dc:	a2b567e3          	bltu	a0,a1,1c00900a <pos_libc_prf+0x124>
1c0095e0:	00241793          	slli	a5,s0,0x2
1c0095e4:	97a2                	add	a5,a5,s0
1c0095e6:	0786                	slli	a5,a5,0x1
1c0095e8:	97b6                	add	a5,a5,a3
1c0095ea:	0017468b          	p.lbu	a3,1(a4!)
1c0095ee:	fd078413          	addi	s0,a5,-48
1c0095f2:	fd068793          	addi	a5,a3,-48
1c0095f6:	89b6                	mv	s3,a3
1c0095f8:	fef674e3          	bgeu	a2,a5,1c0095e0 <pos_libc_prf+0x6fa>
1c0095fc:	8cba                	mv	s9,a4
1c0095fe:	b431                	j	1c00900a <pos_libc_prf+0x124>
1c009600:	4501                	li	a0,0
1c009602:	4601                	li	a2,0
1c009604:	4291                	li	t0,4
1c009606:	40c282b3          	sub	t0,t0,a2
1c00960a:	0132c0fb          	lp.setup	x1,t0,1c009630 <pos_libc_prf+0x74a>
1c00960e:	01ff1713          	slli	a4,t5,0x1f
1c009612:	001ed793          	srli	a5,t4,0x1
1c009616:	8fd9                	or	a5,a5,a4
1c009618:	fc1ebeb3          	p.bclr	t4,t4,30,1
1c00961c:	00fe8e33          	add	t3,t4,a5
1c009620:	001f5f13          	srli	t5,t5,0x1
1c009624:	01de3eb3          	sltu	t4,t3,t4
1c009628:	01ee8733          	add	a4,t4,t5
1c00962c:	8f3a                	mv	t5,a4
1c00962e:	8ef2                	mv	t4,t3
1c009630:	0605                	addi	a2,a2,1
1c009632:	00045363          	bgez	s0,1c009638 <pos_libc_prf+0x752>
1c009636:	4419                	li	s0,6
1c009638:	04700793          	li	a5,71
1c00963c:	0cff8fe3          	beq	t6,a5,1c009f1a <pos_libc_prf+0x1034>
1c009640:	06600793          	li	a5,102
1c009644:	00f69463          	bne	a3,a5,1c00964c <pos_libc_prf+0x766>
1c009648:	6520106f          	j	1c00ac9a <pos_libc_prf+0x1db4>
1c00964c:	00140e93          	addi	t4,s0,1 # 80000001 <__l2_shared_end+0x63fefbf9>
1c009650:	47c1                	li	a5,16
1c009652:	04feceb3          	p.min	t4,t4,a5
1c009656:	1efd                	addi	t4,t4,-1
1c009658:	4581                	li	a1,0
1c00965a:	4781                	li	a5,0
1c00965c:	080002b7          	lui	t0,0x8000
1c009660:	4f15                	li	t5,5
1c009662:	0e85                	addi	t4,t4,1
1c009664:	032ec0fb          	lp.setup	x1,t4,1c0096c8 <pos_libc_prf+0x7e2>
1c009668:	00278813          	addi	a6,a5,2
1c00966c:	00f837b3          	sltu	a5,a6,a5
1c009670:	9796                	add	a5,a5,t0
1c009672:	03e7d2b3          	divu	t0,a5,t5
1c009676:	00385f93          	srli	t6,a6,0x3
1c00967a:	00229893          	slli	a7,t0,0x2
1c00967e:	9896                	add	a7,a7,t0
1c009680:	411787b3          	sub	a5,a5,a7
1c009684:	07f6                	slli	a5,a5,0x1d
1c009686:	01f7efb3          	or	t6,a5,t6
1c00968a:	03efdfb3          	divu	t6,t6,t5
1c00968e:	003f9893          	slli	a7,t6,0x3
1c009692:	005f9793          	slli	a5,t6,0x5
1c009696:	97c6                	add	a5,a5,a7
1c009698:	40f80833          	sub	a6,a6,a5
1c00969c:	03e857b3          	divu	a5,a6,t5
1c0096a0:	01dfdf93          	srli	t6,t6,0x1d
1c0096a4:	9f96                	add	t6,t6,t0
1c0096a6:	98be                	add	a7,a7,a5
1c0096a8:	00f8b833          	sltu	a6,a7,a5
1c0096ac:	987e                	add	a6,a6,t6
1c0096ae:	01f81f93          	slli	t6,a6,0x1f
1c0096b2:	0018d793          	srli	a5,a7,0x1
1c0096b6:	00ffe7b3          	or	a5,t6,a5
1c0096ba:	fc18b8b3          	p.bclr	a7,a7,30,1
1c0096be:	97c6                	add	a5,a5,a7
1c0096c0:	00185813          	srli	a6,a6,0x1
1c0096c4:	0117b8b3          	sltu	a7,a5,a7
1c0096c8:	010882b3          	add	t0,a7,a6
1c0096cc:	97f2                	add	a5,a5,t3
1c0096ce:	9716                	add	a4,a4,t0
1c0096d0:	01c7be33          	sltu	t3,a5,t3
1c0096d4:	00ee02b3          	add	t0,t3,a4
1c0096d8:	f602b733          	p.bclr	a4,t0,27,0
1c0096dc:	c725                	beqz	a4,1c009744 <pos_libc_prf+0x85e>
1c0096de:	00278e93          	addi	t4,a5,2
1c0096e2:	00feb733          	sltu	a4,t4,a5
1c0096e6:	92ba                	add	t0,t0,a4
1c0096e8:	4f95                	li	t6,5
1c0096ea:	03f2df33          	divu	t5,t0,t6
1c0096ee:	003ed713          	srli	a4,t4,0x3
1c0096f2:	0505                	addi	a0,a0,1
1c0096f4:	002f1e13          	slli	t3,t5,0x2
1c0096f8:	9e7a                	add	t3,t3,t5
1c0096fa:	41c282b3          	sub	t0,t0,t3
1c0096fe:	01d29e13          	slli	t3,t0,0x1d
1c009702:	00ee6733          	or	a4,t3,a4
1c009706:	03f75733          	divu	a4,a4,t6
1c00970a:	00371e13          	slli	t3,a4,0x3
1c00970e:	00571793          	slli	a5,a4,0x5
1c009712:	97f2                	add	a5,a5,t3
1c009714:	40fe87b3          	sub	a5,t4,a5
1c009718:	03f7d7b3          	divu	a5,a5,t6
1c00971c:	8375                	srli	a4,a4,0x1d
1c00971e:	977a                	add	a4,a4,t5
1c009720:	9e3e                	add	t3,t3,a5
1c009722:	00fe37b3          	sltu	a5,t3,a5
1c009726:	973e                	add	a4,a4,a5
1c009728:	01f71e93          	slli	t4,a4,0x1f
1c00972c:	001e5793          	srli	a5,t3,0x1
1c009730:	00fee7b3          	or	a5,t4,a5
1c009734:	fc1e3e33          	p.bclr	t3,t3,30,1
1c009738:	97f2                	add	a5,a5,t3
1c00973a:	8305                	srli	a4,a4,0x1
1c00973c:	01c7be33          	sltu	t3,a5,t3
1c009740:	00ee02b3          	add	t0,t3,a4
1c009744:	06600e13          	li	t3,102
1c009748:	00130713          	addi	a4,t1,1
1c00974c:	13c686e3          	beq	a3,t3,1c00a078 <pos_libc_prf+0x1192>
1c009750:	01e7de93          	srli	t4,a5,0x1e
1c009754:	00279613          	slli	a2,a5,0x2
1c009758:	00229e13          	slli	t3,t0,0x2
1c00975c:	97b2                	add	a5,a5,a2
1c00975e:	01ceee33          	or	t3,t4,t3
1c009762:	00c7b633          	sltu	a2,a5,a2
1c009766:	9e16                	add	t3,t3,t0
1c009768:	9672                	add	a2,a2,t3
1c00976a:	0606                	slli	a2,a2,0x1
1c00976c:	01f7de13          	srli	t3,a5,0x1f
1c009770:	00ce6633          	or	a2,t3,a2
1c009774:	01c65e13          	srli	t3,a2,0x1c
1c009778:	c7c63eb3          	p.bclr	t4,a2,3,28
1c00977c:	5632                	lw	a2,44(sp)
1c00977e:	8f72                	mv	t5,t3
1c009780:	030e0e13          	addi	t3,t3,48 # 10030 <pos_soc_event_callback+0xfbec>
1c009784:	01e03f33          	snez	t5,t5
1c009788:	01c30023          	sb	t3,0(t1)
1c00978c:	0786                	slli	a5,a5,0x1
1c00978e:	41e50533          	sub	a0,a0,t5
1c009792:	08061fe3          	bnez	a2,1c00a030 <pos_libc_prf+0x114a>
1c009796:	3c040e63          	beqz	s0,1c009b72 <pos_libc_prf+0xc8c>
1c00979a:	02e00713          	li	a4,46
1c00979e:	00230613          	addi	a2,t1,2
1c0097a2:	00e300a3          	sb	a4,1(t1)
1c0097a6:	00279713          	slli	a4,a5,0x2
1c0097aa:	01e7de13          	srli	t3,a5,0x1e
1c0097ae:	002e9313          	slli	t1,t4,0x2
1c0097b2:	97ba                	add	a5,a5,a4
1c0097b4:	006e6333          	or	t1,t3,t1
1c0097b8:	9376                	add	t1,t1,t4
1c0097ba:	00e7be33          	sltu	t3,a5,a4
1c0097be:	9e1a                	add	t3,t3,t1
1c0097c0:	01f7d713          	srli	a4,a5,0x1f
1c0097c4:	0e06                	slli	t3,t3,0x1
1c0097c6:	01c76e33          	or	t3,a4,t3
1c0097ca:	01ce5713          	srli	a4,t3,0x1c
1c0097ce:	03070713          	addi	a4,a4,48
1c0097d2:	10000337          	lui	t1,0x10000
1c0097d6:	137d                	addi	t1,t1,-1
1c0097d8:	00e60023          	sb	a4,0(a2)
1c0097dc:	006e7e33          	and	t3,t3,t1
1c0097e0:	00179713          	slli	a4,a5,0x1
1c0097e4:	00143463          	p.bneimm	s0,1,1c0097ec <pos_libc_prf+0x906>
1c0097e8:	6240106f          	j	1c00ae0c <pos_libc_prf+0x1f26>
1c0097ec:	078e                	slli	a5,a5,0x3
1c0097ee:	01e75f13          	srli	t5,a4,0x1e
1c0097f2:	002e1e93          	slli	t4,t3,0x2
1c0097f6:	973e                	add	a4,a4,a5
1c0097f8:	01df6eb3          	or	t4,t5,t4
1c0097fc:	9e76                	add	t3,t3,t4
1c0097fe:	00f737b3          	sltu	a5,a4,a5
1c009802:	97f2                	add	a5,a5,t3
1c009804:	0786                	slli	a5,a5,0x1
1c009806:	01f75e13          	srli	t3,a4,0x1f
1c00980a:	00fe67b3          	or	a5,t3,a5
1c00980e:	01c7de13          	srli	t3,a5,0x1c
1c009812:	030e0e13          	addi	t3,t3,48
1c009816:	01c600a3          	sb	t3,1(a2)
1c00981a:	00171e93          	slli	t4,a4,0x1
1c00981e:	0067f7b3          	and	a5,a5,t1
1c009822:	00243463          	p.bneimm	s0,2,1c00982a <pos_libc_prf+0x944>
1c009826:	5dc0106f          	j	1c00ae02 <pos_libc_prf+0x1f1c>
1c00982a:	01eedf13          	srli	t5,t4,0x1e
1c00982e:	070e                	slli	a4,a4,0x3
1c009830:	00279e13          	slli	t3,a5,0x2
1c009834:	9eba                	add	t4,t4,a4
1c009836:	01cf6e33          	or	t3,t5,t3
1c00983a:	00eeb733          	sltu	a4,t4,a4
1c00983e:	97f2                	add	a5,a5,t3
1c009840:	97ba                	add	a5,a5,a4
1c009842:	0786                	slli	a5,a5,0x1
1c009844:	01fed713          	srli	a4,t4,0x1f
1c009848:	8fd9                	or	a5,a5,a4
1c00984a:	01c7de13          	srli	t3,a5,0x1c
1c00984e:	030e0e13          	addi	t3,t3,48
1c009852:	01c60123          	sb	t3,2(a2)
1c009856:	ffd40f13          	addi	t5,s0,-3
1c00985a:	001e9e13          	slli	t3,t4,0x1
1c00985e:	0067f7b3          	and	a5,a5,t1
1c009862:	01e04463          	bgtz	t5,1c00986a <pos_libc_prf+0x984>
1c009866:	5920106f          	j	1c00adf8 <pos_libc_prf+0x1f12>
1c00986a:	003e9713          	slli	a4,t4,0x3
1c00986e:	01ee5f13          	srli	t5,t3,0x1e
1c009872:	00279e93          	slli	t4,a5,0x2
1c009876:	9e3a                	add	t3,t3,a4
1c009878:	01df6eb3          	or	t4,t5,t4
1c00987c:	00ee3733          	sltu	a4,t3,a4
1c009880:	97f6                	add	a5,a5,t4
1c009882:	97ba                	add	a5,a5,a4
1c009884:	0786                	slli	a5,a5,0x1
1c009886:	01fe5713          	srli	a4,t3,0x1f
1c00988a:	8fd9                	or	a5,a5,a4
1c00988c:	01c7d713          	srli	a4,a5,0x1c
1c009890:	03070713          	addi	a4,a4,48
1c009894:	00e601a3          	sb	a4,3(a2)
1c009898:	ffc40e93          	addi	t4,s0,-4
1c00989c:	001e1f13          	slli	t5,t3,0x1
1c0098a0:	0067f7b3          	and	a5,a5,t1
1c0098a4:	01d04463          	bgtz	t4,1c0098ac <pos_libc_prf+0x9c6>
1c0098a8:	5460106f          	j	1c00adee <pos_libc_prf+0x1f08>
1c0098ac:	003e1713          	slli	a4,t3,0x3
1c0098b0:	01ef5e93          	srli	t4,t5,0x1e
1c0098b4:	00279e13          	slli	t3,a5,0x2
1c0098b8:	9f3a                	add	t5,t5,a4
1c0098ba:	01ceee33          	or	t3,t4,t3
1c0098be:	97f2                	add	a5,a5,t3
1c0098c0:	00ef3733          	sltu	a4,t5,a4
1c0098c4:	973e                	add	a4,a4,a5
1c0098c6:	0706                	slli	a4,a4,0x1
1c0098c8:	01ff5793          	srli	a5,t5,0x1f
1c0098cc:	8f5d                	or	a4,a4,a5
1c0098ce:	01c75e13          	srli	t3,a4,0x1c
1c0098d2:	030e0e13          	addi	t3,t3,48
1c0098d6:	01c60223          	sb	t3,4(a2)
1c0098da:	ffb40793          	addi	a5,s0,-5
1c0098de:	001f1e93          	slli	t4,t5,0x1
1c0098e2:	00677733          	and	a4,a4,t1
1c0098e6:	00f04463          	bgtz	a5,1c0098ee <pos_libc_prf+0xa08>
1c0098ea:	4fa0106f          	j	1c00ade4 <pos_libc_prf+0x1efe>
1c0098ee:	003f1793          	slli	a5,t5,0x3
1c0098f2:	00271e13          	slli	t3,a4,0x2
1c0098f6:	01eedf13          	srli	t5,t4,0x1e
1c0098fa:	01cf6e33          	or	t3,t5,t3
1c0098fe:	9ebe                	add	t4,t4,a5
1c009900:	9772                	add	a4,a4,t3
1c009902:	00feb7b3          	sltu	a5,t4,a5
1c009906:	97ba                	add	a5,a5,a4
1c009908:	0786                	slli	a5,a5,0x1
1c00990a:	01fed713          	srli	a4,t4,0x1f
1c00990e:	8fd9                	or	a5,a5,a4
1c009910:	01c7de13          	srli	t3,a5,0x1c
1c009914:	030e0e13          	addi	t3,t3,48
1c009918:	01c602a3          	sb	t3,5(a2)
1c00991c:	ffa40e13          	addi	t3,s0,-6
1c009920:	001e9f13          	slli	t5,t4,0x1
1c009924:	0067f7b3          	and	a5,a5,t1
1c009928:	01c04463          	bgtz	t3,1c009930 <pos_libc_prf+0xa4a>
1c00992c:	4ae0106f          	j	1c00adda <pos_libc_prf+0x1ef4>
1c009930:	003e9713          	slli	a4,t4,0x3
1c009934:	00279e13          	slli	t3,a5,0x2
1c009938:	01ef5e93          	srli	t4,t5,0x1e
1c00993c:	01ceee33          	or	t3,t4,t3
1c009940:	9f3a                	add	t5,t5,a4
1c009942:	97f2                	add	a5,a5,t3
1c009944:	00ef3733          	sltu	a4,t5,a4
1c009948:	973e                	add	a4,a4,a5
1c00994a:	0706                	slli	a4,a4,0x1
1c00994c:	01ff5793          	srli	a5,t5,0x1f
1c009950:	8f5d                	or	a4,a4,a5
1c009952:	01c75e13          	srli	t3,a4,0x1c
1c009956:	030e0e13          	addi	t3,t3,48
1c00995a:	01c60323          	sb	t3,6(a2)
1c00995e:	ff940793          	addi	a5,s0,-7
1c009962:	001f1e93          	slli	t4,t5,0x1
1c009966:	00677733          	and	a4,a4,t1
1c00996a:	00f04463          	bgtz	a5,1c009972 <pos_libc_prf+0xa8c>
1c00996e:	4620106f          	j	1c00add0 <pos_libc_prf+0x1eea>
1c009972:	003f1793          	slli	a5,t5,0x3
1c009976:	00271e13          	slli	t3,a4,0x2
1c00997a:	01eedf13          	srli	t5,t4,0x1e
1c00997e:	01cf6e33          	or	t3,t5,t3
1c009982:	9ebe                	add	t4,t4,a5
1c009984:	9772                	add	a4,a4,t3
1c009986:	00feb7b3          	sltu	a5,t4,a5
1c00998a:	97ba                	add	a5,a5,a4
1c00998c:	0786                	slli	a5,a5,0x1
1c00998e:	01fed713          	srli	a4,t4,0x1f
1c009992:	8fd9                	or	a5,a5,a4
1c009994:	01c7de13          	srli	t3,a5,0x1c
1c009998:	030e0e13          	addi	t3,t3,48
1c00999c:	01c603a3          	sb	t3,7(a2)
1c0099a0:	ff840e13          	addi	t3,s0,-8
1c0099a4:	001e9f13          	slli	t5,t4,0x1
1c0099a8:	0067f7b3          	and	a5,a5,t1
1c0099ac:	01c04463          	bgtz	t3,1c0099b4 <pos_libc_prf+0xace>
1c0099b0:	4160106f          	j	1c00adc6 <pos_libc_prf+0x1ee0>
1c0099b4:	003e9713          	slli	a4,t4,0x3
1c0099b8:	00279e13          	slli	t3,a5,0x2
1c0099bc:	01ef5e93          	srli	t4,t5,0x1e
1c0099c0:	01ceee33          	or	t3,t4,t3
1c0099c4:	9f3a                	add	t5,t5,a4
1c0099c6:	97f2                	add	a5,a5,t3
1c0099c8:	00ef3733          	sltu	a4,t5,a4
1c0099cc:	973e                	add	a4,a4,a5
1c0099ce:	0706                	slli	a4,a4,0x1
1c0099d0:	01ff5793          	srli	a5,t5,0x1f
1c0099d4:	8f5d                	or	a4,a4,a5
1c0099d6:	01c75e13          	srli	t3,a4,0x1c
1c0099da:	030e0e13          	addi	t3,t3,48
1c0099de:	01c60423          	sb	t3,8(a2)
1c0099e2:	ff740793          	addi	a5,s0,-9
1c0099e6:	001f1e93          	slli	t4,t5,0x1
1c0099ea:	00677e33          	and	t3,a4,t1
1c0099ee:	00f04463          	bgtz	a5,1c0099f6 <pos_libc_prf+0xb10>
1c0099f2:	4420106f          	j	1c00ae34 <pos_libc_prf+0x1f4e>
1c0099f6:	003f1793          	slli	a5,t5,0x3
1c0099fa:	01eed713          	srli	a4,t4,0x1e
1c0099fe:	002e1f13          	slli	t5,t3,0x2
1c009a02:	9ebe                	add	t4,t4,a5
1c009a04:	01e76f33          	or	t5,a4,t5
1c009a08:	9e7a                	add	t3,t3,t5
1c009a0a:	00feb733          	sltu	a4,t4,a5
1c009a0e:	9772                	add	a4,a4,t3
1c009a10:	01fed793          	srli	a5,t4,0x1f
1c009a14:	0706                	slli	a4,a4,0x1
1c009a16:	8f5d                	or	a4,a4,a5
1c009a18:	01c75793          	srli	a5,a4,0x1c
1c009a1c:	03078793          	addi	a5,a5,48
1c009a20:	00f604a3          	sb	a5,9(a2)
1c009a24:	ff640e13          	addi	t3,s0,-10
1c009a28:	001e9793          	slli	a5,t4,0x1
1c009a2c:	00677733          	and	a4,a4,t1
1c009a30:	01c04463          	bgtz	t3,1c009a38 <pos_libc_prf+0xb52>
1c009a34:	3f60106f          	j	1c00ae2a <pos_libc_prf+0x1f44>
1c009a38:	00279313          	slli	t1,a5,0x2
1c009a3c:	01e7de93          	srli	t4,a5,0x1e
1c009a40:	00271e13          	slli	t3,a4,0x2
1c009a44:	979a                	add	a5,a5,t1
1c009a46:	01ceee33          	or	t3,t4,t3
1c009a4a:	0067b333          	sltu	t1,a5,t1
1c009a4e:	9772                	add	a4,a4,t3
1c009a50:	971a                	add	a4,a4,t1
1c009a52:	0706                	slli	a4,a4,0x1
1c009a54:	01f7d313          	srli	t1,a5,0x1f
1c009a58:	00e36733          	or	a4,t1,a4
1c009a5c:	01c75e13          	srli	t3,a4,0x1c
1c009a60:	030e0e13          	addi	t3,t3,48
1c009a64:	10000337          	lui	t1,0x10000
1c009a68:	137d                	addi	t1,t1,-1
1c009a6a:	01c60523          	sb	t3,10(a2)
1c009a6e:	ff540e13          	addi	t3,s0,-11
1c009a72:	00179f13          	slli	t5,a5,0x1
1c009a76:	00677733          	and	a4,a4,t1
1c009a7a:	01c04463          	bgtz	t3,1c009a82 <pos_libc_prf+0xb9c>
1c009a7e:	3a20106f          	j	1c00ae20 <pos_libc_prf+0x1f3a>
1c009a82:	01ef5e93          	srli	t4,t5,0x1e
1c009a86:	078e                	slli	a5,a5,0x3
1c009a88:	00271e13          	slli	t3,a4,0x2
1c009a8c:	9f3e                	add	t5,t5,a5
1c009a8e:	01ceee33          	or	t3,t4,t3
1c009a92:	9772                	add	a4,a4,t3
1c009a94:	00ff37b3          	sltu	a5,t5,a5
1c009a98:	97ba                	add	a5,a5,a4
1c009a9a:	0786                	slli	a5,a5,0x1
1c009a9c:	01ff5713          	srli	a4,t5,0x1f
1c009aa0:	8fd9                	or	a5,a5,a4
1c009aa2:	01c7de13          	srli	t3,a5,0x1c
1c009aa6:	030e0e13          	addi	t3,t3,48
1c009aaa:	01c605a3          	sb	t3,11(a2)
1c009aae:	ff440e13          	addi	t3,s0,-12
1c009ab2:	001f1e93          	slli	t4,t5,0x1
1c009ab6:	0067f7b3          	and	a5,a5,t1
1c009aba:	01c04463          	bgtz	t3,1c009ac2 <pos_libc_prf+0xbdc>
1c009abe:	3580106f          	j	1c00ae16 <pos_libc_prf+0x1f30>
1c009ac2:	003f1713          	slli	a4,t5,0x3
1c009ac6:	00279e13          	slli	t3,a5,0x2
1c009aca:	01eedf13          	srli	t5,t4,0x1e
1c009ace:	01cf6e33          	or	t3,t5,t3
1c009ad2:	9eba                	add	t4,t4,a4
1c009ad4:	97f2                	add	a5,a5,t3
1c009ad6:	00eeb733          	sltu	a4,t4,a4
1c009ada:	973e                	add	a4,a4,a5
1c009adc:	0706                	slli	a4,a4,0x1
1c009ade:	01fed793          	srli	a5,t4,0x1f
1c009ae2:	8f5d                	or	a4,a4,a5
1c009ae4:	01c75e13          	srli	t3,a4,0x1c
1c009ae8:	030e0e13          	addi	t3,t3,48
1c009aec:	01c60623          	sb	t3,12(a2)
1c009af0:	ff340793          	addi	a5,s0,-13
1c009af4:	001e9e13          	slli	t3,t4,0x1
1c009af8:	00677733          	and	a4,a4,t1
1c009afc:	00f04463          	bgtz	a5,1c009b04 <pos_libc_prf+0xc1e>
1c009b00:	3480106f          	j	1c00ae48 <pos_libc_prf+0x1f62>
1c009b04:	003e9793          	slli	a5,t4,0x3
1c009b08:	01ee5f13          	srli	t5,t3,0x1e
1c009b0c:	00271e93          	slli	t4,a4,0x2
1c009b10:	9e3e                	add	t3,t3,a5
1c009b12:	01df6eb3          	or	t4,t5,t4
1c009b16:	9776                	add	a4,a4,t4
1c009b18:	00fe37b3          	sltu	a5,t3,a5
1c009b1c:	97ba                	add	a5,a5,a4
1c009b1e:	0786                	slli	a5,a5,0x1
1c009b20:	01fe5713          	srli	a4,t3,0x1f
1c009b24:	8fd9                	or	a5,a5,a4
1c009b26:	01c7d713          	srli	a4,a5,0x1c
1c009b2a:	03070713          	addi	a4,a4,48
1c009b2e:	00e606a3          	sb	a4,13(a2)
1c009b32:	ff240e93          	addi	t4,s0,-14
1c009b36:	001e1713          	slli	a4,t3,0x1
1c009b3a:	0067f7b3          	and	a5,a5,t1
1c009b3e:	01d04463          	bgtz	t4,1c009b46 <pos_libc_prf+0xc60>
1c009b42:	2fc0106f          	j	1c00ae3e <pos_libc_prf+0x1f58>
1c009b46:	003e1313          	slli	t1,t3,0x3
1c009b4a:	01e75e93          	srli	t4,a4,0x1e
1c009b4e:	00279e13          	slli	t3,a5,0x2
1c009b52:	01ceee33          	or	t3,t4,t3
1c009b56:	971a                	add	a4,a4,t1
1c009b58:	00673733          	sltu	a4,a4,t1
1c009b5c:	97f2                	add	a5,a5,t3
1c009b5e:	97ba                	add	a5,a5,a4
1c009b60:	0786                	slli	a5,a5,0x1
1c009b62:	83f1                	srli	a5,a5,0x1c
1c009b64:	03078793          	addi	a5,a5,48
1c009b68:	00f60713          	addi	a4,a2,15
1c009b6c:	00f60723          	sb	a5,14(a2)
1c009b70:	1445                	addi	s0,s0,-15
1c009b72:	5e0584e3          	beqz	a1,1c00a95a <pos_libc_prf+0x1a74>
1c009b76:	4e81                	li	t4,0
1c009b78:	d402                	sw	zero,40(sp)
1c009b7a:	cc02                	sw	zero,24(sp)
1c009b7c:	03000593          	li	a1,48
1c009b80:	fff74783          	lbu	a5,-1(a4)
1c009b84:	863a                	mv	a2,a4
1c009b86:	177d                	addi	a4,a4,-1
1c009b88:	feb78ce3          	beq	a5,a1,1c009b80 <pos_libc_prf+0xc9a>
1c009b8c:	02e00593          	li	a1,46
1c009b90:	4401                	li	s0,0
1c009b92:	00b78363          	beq	a5,a1,1c009b98 <pos_libc_prf+0xcb2>
1c009b96:	8732                	mv	a4,a2
1c009b98:	0df6f793          	andi	a5,a3,223
1c009b9c:	04500613          	li	a2,69
1c009ba0:	3ac78563          	beq	a5,a2,1c009f4a <pos_libc_prf+0x1064>
1c009ba4:	185c                	addi	a5,sp,52
1c009ba6:	40f709b3          	sub	s3,a4,a5
1c009baa:	00070023          	sb	zero,0(a4)
1c009bae:	99f6                	add	s3,s3,t4
1c009bb0:	47d2                	lw	a5,20(sp)
1c009bb2:	2e079563          	bnez	a5,1c009e9c <pos_libc_prf+0xfb6>
1c009bb6:	47c2                	lw	a5,16(sp)
1c009bb8:	2e079263          	bnez	a5,1c009e9c <pos_libc_prf+0xfb6>
1c009bbc:	03414783          	lbu	a5,52(sp)
1c009bc0:	02d00713          	li	a4,45
1c009bc4:	c802                	sw	zero,16(sp)
1c009bc6:	2ce78b63          	beq	a5,a4,1c009e9c <pos_libc_prf+0xfb6>
1c009bca:	fd078793          	addi	a5,a5,-48
1c009bce:	4725                	li	a4,9
1c009bd0:	413d8933          	sub	s2,s11,s3
1c009bd4:	f4f77263          	bgeu	a4,a5,1c009318 <pos_libc_prf+0x432>
1c009bd8:	03410c13          	addi	s8,sp,52
1c009bdc:	ca02                	sw	zero,20(sp)
1c009bde:	d58ff06f          	j	1c009136 <pos_libc_prf+0x250>
1c009be2:	47c2                	lw	a5,16(sp)
1c009be4:	41340433          	sub	s0,s0,s3
1c009be8:	03410c13          	addi	s8,sp,52
1c009bec:	97a2                	add	a5,a5,s0
1c009bee:	0407e7b3          	p.max	a5,a5,zero
1c009bf2:	ca3e                	sw	a5,20(sp)
1c009bf4:	97ce                	add	a5,a5,s3
1c009bf6:	40fd8933          	sub	s2,s11,a5
1c009bfa:	d36ff06f          	j	1c009130 <pos_libc_prf+0x24a>
1c009bfe:	47e2                	lw	a5,24(sp)
1c009c00:	079d                	addi	a5,a5,7
1c009c02:	c407b7b3          	p.bclr	a5,a5,2,0
1c009c06:	8d3e                	mv	s10,a5
1c009c08:	008d258b          	p.lw	a1,8(s10!)
1c009c0c:	43dc                	lw	a5,4(a5)
1c009c0e:	0155d513          	srli	a0,a1,0x15
1c009c12:	00b79713          	slli	a4,a5,0xb
1c009c16:	8f49                	or	a4,a4,a0
1c009c18:	0147d613          	srli	a2,a5,0x14
1c009c1c:	e8b63633          	p.bclr	a2,a2,20,11
1c009c20:	00b59513          	slli	a0,a1,0xb
1c009c24:	c1f73733          	p.bclr	a4,a4,0,31
1c009c28:	3807cf63          	bltz	a5,1c009fc6 <pos_libc_prf+0x10e0>
1c009c2c:	47d2                	lw	a5,20(sp)
1c009c2e:	42078963          	beqz	a5,1c00a060 <pos_libc_prf+0x117a>
1c009c32:	02b00793          	li	a5,43
1c009c36:	02f10a23          	sb	a5,52(sp)
1c009c3a:	03510313          	addi	t1,sp,53
1c009c3e:	7ff00793          	li	a5,2047
1c009c42:	64f60ce3          	beq	a2,a5,1c00aa9a <pos_libc_prf+0x1bb4>
1c009c46:	04600793          	li	a5,70
1c009c4a:	0df6ff93          	andi	t6,a3,223
1c009c4e:	00f69663          	bne	a3,a5,1c009c5a <pos_libc_prf+0xd74>
1c009c52:	04600f93          	li	t6,70
1c009c56:	06600693          	li	a3,102
1c009c5a:	41f65593          	srai	a1,a2,0x1f
1c009c5e:	00a66eb3          	or	t4,a2,a0
1c009c62:	00e5ef33          	or	t5,a1,a4
1c009c66:	01eee7b3          	or	a5,t4,t5
1c009c6a:	98078be3          	beqz	a5,1c009600 <pos_libc_prf+0x71a>
1c009c6e:	6a0611e3          	bnez	a2,1c00ab10 <pos_libc_prf+0x1c2a>
1c009c72:	01f55793          	srli	a5,a0,0x1f
1c009c76:	00171f13          	slli	t5,a4,0x1
1c009c7a:	01e7ef33          	or	t5,a5,t5
1c009c7e:	00151e93          	slli	t4,a0,0x1
1c009c82:	000f4c63          	bltz	t5,1c009c9a <pos_libc_prf+0xdb4>
1c009c86:	01fed793          	srli	a5,t4,0x1f
1c009c8a:	001f1713          	slli	a4,t5,0x1
1c009c8e:	167d                	addi	a2,a2,-1
1c009c90:	00e7ef33          	or	t5,a5,a4
1c009c94:	0e86                	slli	t4,t4,0x1
1c009c96:	fe0758e3          	bgez	a4,1c009c86 <pos_libc_prf+0xda0>
1c009c9a:	c0260613          	addi	a2,a2,-1022
1c009c9e:	33333e37          	lui	t3,0x33333
1c009ca2:	800002b7          	lui	t0,0x80000
1c009ca6:	4501                	li	a0,0
1c009ca8:	332e0e13          	addi	t3,t3,818 # 33333332 <__l2_shared_end+0x17322f2a>
1c009cac:	fff2c293          	not	t0,t0
1c009cb0:	53f9                	li	t2,-2
1c009cb2:	01ff1593          	slli	a1,t5,0x1f
1c009cb6:	001ed713          	srli	a4,t4,0x1
1c009cba:	fc1eb7b3          	p.bclr	a5,t4,30,1
1c009cbe:	8f4d                	or	a4,a4,a1
1c009cc0:	00e78eb3          	add	t4,a5,a4
1c009cc4:	00feb7b3          	sltu	a5,t4,a5
1c009cc8:	001f5f13          	srli	t5,t5,0x1
1c009ccc:	9f3e                	add	t5,t5,a5
1c009cce:	87b2                	mv	a5,a2
1c009cd0:	0605                	addi	a2,a2,1
1c009cd2:	ffee60e3          	bltu	t3,t5,1c009cb2 <pos_libc_prf+0xdcc>
1c009cd6:	002e9613          	slli	a2,t4,0x2
1c009cda:	01eed593          	srli	a1,t4,0x1e
1c009cde:	002f1713          	slli	a4,t5,0x2
1c009ce2:	9eb2                	add	t4,t4,a2
1c009ce4:	8f4d                	or	a4,a4,a1
1c009ce6:	00ceb633          	sltu	a2,t4,a2
1c009cea:	9f3a                	add	t5,t5,a4
1c009cec:	9f32                	add	t5,t5,a2
1c009cee:	01fed713          	srli	a4,t4,0x1f
1c009cf2:	00278613          	addi	a2,a5,2
1c009cf6:	157d                	addi	a0,a0,-1
1c009cf8:	005f1763          	bne	t5,t0,1c009d06 <pos_libc_prf+0xe20>
1c009cfc:	00178613          	addi	a2,a5,1
1c009d00:	0e86                	slli	t4,t4,0x1
1c009d02:	fc174f33          	p.bset	t5,a4,30,1
1c009d06:	fa7646e3          	blt	a2,t2,1c009cb2 <pos_libc_prf+0xdcc>
1c009d0a:	80000e37          	lui	t3,0x80000
1c009d0e:	4295                	li	t0,5
1c009d10:	fffe4e13          	not	t3,t3
1c009d14:	8ec058e3          	blez	a2,1c009604 <pos_libc_prf+0x71e>
1c009d18:	002e8713          	addi	a4,t4,2
1c009d1c:	01d73eb3          	sltu	t4,a4,t4
1c009d20:	9f76                	add	t5,t5,t4
1c009d22:	025f57b3          	divu	a5,t5,t0
1c009d26:	00375593          	srli	a1,a4,0x3
1c009d2a:	167d                	addi	a2,a2,-1
1c009d2c:	0505                	addi	a0,a0,1
1c009d2e:	00279813          	slli	a6,a5,0x2
1c009d32:	983e                	add	a6,a6,a5
1c009d34:	410f0f33          	sub	t5,t5,a6
1c009d38:	0f76                	slli	t5,t5,0x1d
1c009d3a:	00bf65b3          	or	a1,t5,a1
1c009d3e:	0255d5b3          	divu	a1,a1,t0
1c009d42:	00359e93          	slli	t4,a1,0x3
1c009d46:	00559813          	slli	a6,a1,0x5
1c009d4a:	9876                	add	a6,a6,t4
1c009d4c:	41070733          	sub	a4,a4,a6
1c009d50:	02575733          	divu	a4,a4,t0
1c009d54:	01d5df13          	srli	t5,a1,0x1d
1c009d58:	97fa                	add	a5,a5,t5
1c009d5a:	9eba                	add	t4,t4,a4
1c009d5c:	00eeb733          	sltu	a4,t4,a4
1c009d60:	00f70f33          	add	t5,a4,a5
1c009d64:	01fed793          	srli	a5,t4,0x1f
1c009d68:	0f06                	slli	t5,t5,0x1
1c009d6a:	01e7ef33          	or	t5,a5,t5
1c009d6e:	0e86                	slli	t4,t4,0x1
1c009d70:	167d                	addi	a2,a2,-1
1c009d72:	ffee79e3          	bgeu	t3,t5,1c009d64 <pos_libc_prf+0xe7e>
1c009d76:	bf79                	j	1c009d14 <pos_libc_prf+0xe2e>
1c009d78:	4d62                	lw	s10,24(sp)
1c009d7a:	47f2                	lw	a5,28(sp)
1c009d7c:	4681                	li	a3,0
1c009d7e:	004d258b          	p.lw	a1,4(s10!)
1c009d82:	02f11a23          	sh	a5,52(sp)
1c009d86:	03610513          	addi	a0,sp,54
1c009d8a:	4ea5                	li	t4,9
1c009d8c:	4f3d                	li	t5,15
1c009d8e:	a039                	j	1c009d9c <pos_libc_prf+0xeb6>
1c009d90:	011980ab          	p.sb	a7,1(s3!)
1c009d94:	c69d                	beqz	a3,1c009dc2 <pos_libc_prf+0xedc>
1c009d96:	85b2                	mv	a1,a2
1c009d98:	86ba                	mv	a3,a4
1c009d9a:	854e                	mv	a0,s3
1c009d9c:	f645b7b3          	p.bclr	a5,a1,27,4
1c009da0:	01c69713          	slli	a4,a3,0x1c
1c009da4:	0045d613          	srli	a2,a1,0x4
1c009da8:	8e59                	or	a2,a2,a4
1c009daa:	89aa                	mv	s3,a0
1c009dac:	03078893          	addi	a7,a5,48
1c009db0:	05778813          	addi	a6,a5,87
1c009db4:	0046d713          	srli	a4,a3,0x4
1c009db8:	fcfefce3          	bgeu	t4,a5,1c009d90 <pos_libc_prf+0xeaa>
1c009dbc:	010980ab          	p.sb	a6,1(s3!)
1c009dc0:	faf9                	bnez	a3,1c009d96 <pos_libc_prf+0xeb0>
1c009dc2:	fcbf6ae3          	bltu	t5,a1,1c009d96 <pos_libc_prf+0xeb0>
1c009dc6:	00098023          	sb	zero,0(s3)
1c009dca:	03610793          	addi	a5,sp,54
1c009dce:	0aa7fd63          	bgeu	a5,a0,1c009e88 <pos_libc_prf+0xfa2>
1c009dd2:	5782                	lw	a5,32(sp)
1c009dd4:	1858                	addi	a4,sp,52
1c009dd6:	00a785b3          	add	a1,a5,a0
1c009dda:	0015d693          	srli	a3,a1,0x1
1c009dde:	00368793          	addi	a5,a3,3
1c009de2:	97ba                	add	a5,a5,a4
1c009de4:	40d50633          	sub	a2,a0,a3
1c009de8:	04c7b633          	p.sletu	a2,a5,a2
1c009dec:	03610793          	addi	a5,sp,54
1c009df0:	04f9b7b3          	p.sletu	a5,s3,a5
1c009df4:	8e5d                	or	a2,a2,a5
1c009df6:	872a                	mv	a4,a0
1c009df8:	03610793          	addi	a5,sp,54
1c009dfc:	62060ee3          	beqz	a2,1c00ac38 <pos_libc_prf+0x1d52>
1c009e00:	0065b593          	sltiu	a1,a1,6
1c009e04:	62059ae3          	bnez	a1,1c00ac38 <pos_libc_prf+0x1d52>
1c009e08:	00168313          	addi	t1,a3,1
1c009e0c:	c2033633          	p.bclr	a2,t1,1,0
1c009e10:	1671                	addi	a2,a2,-4
1c009e12:	8209                	srli	a2,a2,0x2
1c009e14:	ffd50593          	addi	a1,a0,-3
1c009e18:	0605                	addi	a2,a2,1
1c009e1a:	00a640fb          	lp.setup	x1,a2,1c009e2e <pos_libc_prf+0xf48>
1c009e1e:	4394                	lw	a3,0(a5)
1c009e20:	4198                	lw	a4,0(a1)
1c009e22:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c009e26:	fed5ae2b          	p.sw	a3,-4(a1!)
1c009e2a:	c2d77757          	pv.shufflei0.sci.b	a4,a4,27
1c009e2e:	00e7a22b          	p.sw	a4,4(a5!)
1c009e32:	c20336b3          	p.bclr	a3,t1,1,0
1c009e36:	03610713          	addi	a4,sp,54
1c009e3a:	40d507b3          	sub	a5,a0,a3
1c009e3e:	9736                	add	a4,a4,a3
1c009e40:	04668463          	beq	a3,t1,1c009e88 <pos_libc_prf+0xfa2>
1c009e44:	00074603          	lbu	a2,0(a4)
1c009e48:	0007c583          	lbu	a1,0(a5)
1c009e4c:	86be                	mv	a3,a5
1c009e4e:	fec68fab          	p.sb	a2,-1(a3!)
1c009e52:	863a                	mv	a2,a4
1c009e54:	00b600ab          	p.sb	a1,1(a2!)
1c009e58:	02d67863          	bgeu	a2,a3,1c009e88 <pos_libc_prf+0xfa2>
1c009e5c:	00174583          	lbu	a1,1(a4)
1c009e60:	fff7c603          	lbu	a2,-1(a5)
1c009e64:	00270693          	addi	a3,a4,2
1c009e68:	feb78fa3          	sb	a1,-1(a5)
1c009e6c:	00c700a3          	sb	a2,1(a4)
1c009e70:	ffe78613          	addi	a2,a5,-2
1c009e74:	00c6fa63          	bgeu	a3,a2,1c009e88 <pos_libc_prf+0xfa2>
1c009e78:	00274603          	lbu	a2,2(a4)
1c009e7c:	ffe7c683          	lbu	a3,-2(a5)
1c009e80:	fec78f23          	sb	a2,-2(a5)
1c009e84:	00d70123          	sb	a3,2(a4)
1c009e88:	185c                	addi	a5,sp,52
1c009e8a:	40f989b3          	sub	s3,s3,a5
1c009e8e:	4789                	li	a5,2
1c009e90:	c83e                	sw	a5,16(sp)
1c009e92:	c78ff06f          	j	1c00930a <pos_libc_prf+0x424>
1c009e96:	4b01                	li	s6,0
1c009e98:	908ff06f          	j	1c008fa0 <pos_libc_prf+0xba>
1c009e9c:	4705                	li	a4,1
1c009e9e:	03514783          	lbu	a5,53(sp)
1c009ea2:	c83a                	sw	a4,16(sp)
1c009ea4:	b31d                	j	1c009bca <pos_libc_prf+0xce4>
1c009ea6:	01672023          	sw	s6,0(a4)
1c009eaa:	896ff06f          	j	1c008f40 <pos_libc_prf+0x5a>
1c009eae:	4785                	li	a5,1
1c009eb0:	c83e                	sw	a5,16(sp)
1c009eb2:	c58ff06f          	j	1c00930a <pos_libc_prf+0x424>
1c009eb6:	8aa2                	mv	s5,s0
1c009eb8:	8bb6                	mv	s7,a3
1c009eba:	a029                	j	1c009ec4 <pos_libc_prf+0xfde>
1c009ebc:	9a02                	jalr	s4
1c009ebe:	1afd                	addi	s5,s5,-1
1c009ec0:	a1f52363          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c009ec4:	85a6                	mv	a1,s1
1c009ec6:	03000513          	li	a0,48
1c009eca:	ff5049e3          	bgtz	s5,1c009ebc <pos_libc_prf+0xfd6>
1c009ece:	408b8c33          	sub	s8,s7,s0
1c009ed2:	8aee                	mv	s5,s11
1c009ed4:	9c56                	add	s8,s8,s5
1c009ed6:	a031                	j	1c009ee2 <pos_libc_prf+0xffc>
1c009ed8:	001ac50b          	p.lbu	a0,1(s5!)
1c009edc:	9a02                	jalr	s4
1c009ede:	9ff52463          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c009ee2:	415c07b3          	sub	a5,s8,s5
1c009ee6:	85a6                	mv	a1,s1
1c009ee8:	fef048e3          	bgtz	a5,1c009ed8 <pos_libc_prf+0xff2>
1c009eec:	47c2                	lw	a5,16(sp)
1c009eee:	cc6a                	sw	s10,24(sp)
1c009ef0:	9b3e                	add	s6,s6,a5
1c009ef2:	47d2                	lw	a5,20(sp)
1c009ef4:	9b3e                	add	s6,s6,a5
1c009ef6:	9b4e                	add	s6,s6,s3
1c009ef8:	85205463          	blez	s2,1c008f40 <pos_libc_prf+0x5a>
1c009efc:	fff90413          	addi	s0,s2,-1
1c009f00:	85a6                	mv	a1,s1
1c009f02:	02000513          	li	a0,32
1c009f06:	9a02                	jalr	s4
1c009f08:	147d                	addi	s0,s0,-1
1c009f0a:	9bf52e63          	p.beqimm	a0,-1,1c0090c6 <pos_libc_prf+0x1e0>
1c009f0e:	fff439e3          	p.bneimm	s0,-1,1c009f00 <pos_libc_prf+0x101a>
1c009f12:	9b4a                	add	s6,s6,s2
1c009f14:	cc6a                	sw	s10,24(sp)
1c009f16:	82aff06f          	j	1c008f40 <pos_libc_prf+0x5a>
1c009f1a:	57f5                	li	a5,-3
1c009f1c:	0ef54963          	blt	a0,a5,1c00a00e <pos_libc_prf+0x1128>
1c009f20:	0ea44763          	blt	s0,a0,1c00a00e <pos_libc_prf+0x1128>
1c009f24:	57b2                	lw	a5,44(sp)
1c009f26:	40a406b3          	sub	a3,s0,a0
1c009f2a:	620791e3          	bnez	a5,1c00ad4c <pos_libc_prf+0x1e66>
1c009f2e:	87a2                	mv	a5,s0
1c009f30:	00d035b3          	snez	a1,a3
1c009f34:	8436                	mv	s0,a3
1c009f36:	58f05fe3          	blez	a5,1c00acd4 <pos_libc_prf+0x1dee>
1c009f3a:	46c1                	li	a3,16
1c009f3c:	04d7ceb3          	p.min	t4,a5,a3
1c009f40:	1efd                	addi	t4,t4,-1
1c009f42:	06600693          	li	a3,102
1c009f46:	f14ff06f          	j	1c00965a <pos_libc_prf+0x774>
1c009f4a:	863a                	mv	a2,a4
1c009f4c:	00d6012b          	p.sb	a3,2(a2!)
1c009f50:	02b00793          	li	a5,43
1c009f54:	00055663          	bgez	a0,1c009f60 <pos_libc_prf+0x107a>
1c009f58:	40a00533          	neg	a0,a0
1c009f5c:	02d00793          	li	a5,45
1c009f60:	00f700a3          	sb	a5,1(a4)
1c009f64:	06300793          	li	a5,99
1c009f68:	00a7de63          	bge	a5,a0,1c009f84 <pos_libc_prf+0x109e>
1c009f6c:	06400693          	li	a3,100
1c009f70:	02d547b3          	div	a5,a0,a3
1c009f74:	00370613          	addi	a2,a4,3
1c009f78:	02d56533          	rem	a0,a0,a3
1c009f7c:	03078793          	addi	a5,a5,48
1c009f80:	00f70123          	sb	a5,2(a4)
1c009f84:	47a9                	li	a5,10
1c009f86:	8732                	mv	a4,a2
1c009f88:	02f546b3          	div	a3,a0,a5
1c009f8c:	02f567b3          	rem	a5,a0,a5
1c009f90:	03068693          	addi	a3,a3,48
1c009f94:	00d7012b          	p.sb	a3,2(a4!)
1c009f98:	03078793          	addi	a5,a5,48
1c009f9c:	00f600a3          	sb	a5,1(a2)
1c009fa0:	b111                	j	1c009ba4 <pos_libc_prf+0xcbe>
1c009fa2:	00f03733          	snez	a4,a5
1c009fa6:	40d009b3          	neg	s3,a3
1c009faa:	40e989b3          	sub	s3,s3,a4
1c009fae:	02d00693          	li	a3,45
1c009fb2:	03510713          	addi	a4,sp,53
1c009fb6:	02d10a23          	sb	a3,52(sp)
1c009fba:	40f007b3          	neg	a5,a5
1c009fbe:	cc3a                	sw	a4,24(sp)
1c009fc0:	893a                	mv	s2,a4
1c009fc2:	a44ff06f          	j	1c009206 <pos_libc_prf+0x320>
1c009fc6:	02d00793          	li	a5,45
1c009fca:	02f10a23          	sb	a5,52(sp)
1c009fce:	03510313          	addi	t1,sp,53
1c009fd2:	b1b5                	j	1c009c3e <pos_libc_prf+0xd58>
1c009fd4:	000ac503          	lbu	a0,0(s5)
1c009fd8:	47a5                	li	a5,9
1c009fda:	fd050713          	addi	a4,a0,-48
1c009fde:	48e7ece3          	bltu	a5,a4,1c00ac76 <pos_libc_prf+0x1d90>
1c009fe2:	001a8d93          	addi	s11,s5,1
1c009fe6:	013a8bb3          	add	s7,s5,s3
1c009fea:	a801                	j	1c009ffa <pos_libc_prf+0x1114>
1c009fec:	001dc50b          	p.lbu	a0,1(s11!)
1c009ff0:	4725                	li	a4,9
1c009ff2:	fd050793          	addi	a5,a0,-48
1c009ff6:	48f760e3          	bltu	a4,a5,1c00ac76 <pos_libc_prf+0x1d90>
1c009ffa:	85a6                	mv	a1,s1
1c009ffc:	41bb8c33          	sub	s8,s7,s11
1c00a000:	8aee                	mv	s5,s11
1c00a002:	9a02                	jalr	s4
1c00a004:	fff534e3          	p.bneimm	a0,-1,1c009fec <pos_libc_prf+0x1106>
1c00a008:	5b7d                	li	s6,-1
1c00a00a:	f97fe06f          	j	1c008fa0 <pos_libc_prf+0xba>
1c00a00e:	16f9                	addi	a3,a3,-2
1c00a010:	0ff6f693          	andi	a3,a3,255
1c00a014:	180400e3          	beqz	s0,1c00a994 <pos_libc_prf+0x1aae>
1c00a018:	47c1                	li	a5,16
1c00a01a:	04f44eb3          	p.min	t4,s0,a5
1c00a01e:	57b2                	lw	a5,44(sp)
1c00a020:	147d                	addi	s0,s0,-1
1c00a022:	1efd                	addi	t4,t4,-1
1c00a024:	160799e3          	bnez	a5,1c00a996 <pos_libc_prf+0x1ab0>
1c00a028:	008035b3          	snez	a1,s0
1c00a02c:	e2eff06f          	j	1c00965a <pos_libc_prf+0x774>
1c00a030:	02e00713          	li	a4,46
1c00a034:	00e300a3          	sb	a4,1(t1) # 10000001 <__l1_heap_size+0xffc0021>
1c00a038:	00230613          	addi	a2,t1,2
1c00a03c:	f6041563          	bnez	s0,1c0097a6 <pos_libc_prf+0x8c0>
1c00a040:	8732                	mv	a4,a2
1c00a042:	be05                	j	1c009b72 <pos_libc_prf+0xc8c>
1c00a044:	4742                	lw	a4,16(sp)
1c00a046:	89b6                	mv	s3,a3
1c00a048:	44070ee3          	beqz	a4,1c00aca4 <pos_libc_prf+0x1dbe>
1c00a04c:	02000713          	li	a4,32
1c00a050:	02e10a23          	sb	a4,52(sp)
1c00a054:	03510713          	addi	a4,sp,53
1c00a058:	cc3a                	sw	a4,24(sp)
1c00a05a:	893a                	mv	s2,a4
1c00a05c:	9aaff06f          	j	1c009206 <pos_libc_prf+0x320>
1c00a060:	47c2                	lw	a5,16(sp)
1c00a062:	03410313          	addi	t1,sp,52
1c00a066:	bc078ce3          	beqz	a5,1c009c3e <pos_libc_prf+0xd58>
1c00a06a:	02000793          	li	a5,32
1c00a06e:	02f10a23          	sb	a5,52(sp)
1c00a072:	03510313          	addi	t1,sp,53
1c00a076:	b6e1                	j	1c009c3e <pos_libc_prf+0xd58>
1c00a078:	46a055e3          	blez	a0,1c00ace2 <pos_libc_prf+0x1dfc>
1c00a07c:	00279e13          	slli	t3,a5,0x2
1c00a080:	01e7df93          	srli	t6,a5,0x1e
1c00a084:	00229f13          	slli	t5,t0,0x2
1c00a088:	00fe0eb3          	add	t4,t3,a5
1c00a08c:	01efef33          	or	t5,t6,t5
1c00a090:	9f16                	add	t5,t5,t0
1c00a092:	01ceb2b3          	sltu	t0,t4,t3
1c00a096:	92fa                	add	t0,t0,t5
1c00a098:	01fed793          	srli	a5,t4,0x1f
1c00a09c:	0286                	slli	t0,t0,0x1
1c00a09e:	0057e2b3          	or	t0,a5,t0
1c00a0a2:	01c2d793          	srli	a5,t0,0x1c
1c00a0a6:	03078793          	addi	a5,a5,48
1c00a0aa:	10000e37          	lui	t3,0x10000
1c00a0ae:	1e7d                	addi	t3,t3,-1
1c00a0b0:	00f30023          	sb	a5,0(t1)
1c00a0b4:	01c2f2b3          	and	t0,t0,t3
1c00a0b8:	001e9793          	slli	a5,t4,0x1
1c00a0bc:	621529e3          	p.beqimm	a0,1,1c00aeee <pos_libc_prf+0x2008>
1c00a0c0:	01e7df93          	srli	t6,a5,0x1e
1c00a0c4:	00229f13          	slli	t5,t0,0x2
1c00a0c8:	003e9713          	slli	a4,t4,0x3
1c00a0cc:	00f70eb3          	add	t4,a4,a5
1c00a0d0:	01efef33          	or	t5,t6,t5
1c00a0d4:	9f16                	add	t5,t5,t0
1c00a0d6:	00eeb733          	sltu	a4,t4,a4
1c00a0da:	977a                	add	a4,a4,t5
1c00a0dc:	01fed793          	srli	a5,t4,0x1f
1c00a0e0:	0706                	slli	a4,a4,0x1
1c00a0e2:	8f5d                	or	a4,a4,a5
1c00a0e4:	01c75793          	srli	a5,a4,0x1c
1c00a0e8:	03078793          	addi	a5,a5,48
1c00a0ec:	00f300a3          	sb	a5,1(t1)
1c00a0f0:	01c772b3          	and	t0,a4,t3
1c00a0f4:	001e9793          	slli	a5,t4,0x1
1c00a0f8:	5e2525e3          	p.beqimm	a0,2,1c00aee2 <pos_libc_prf+0x1ffc>
1c00a0fc:	01e7df93          	srli	t6,a5,0x1e
1c00a100:	00229f13          	slli	t5,t0,0x2
1c00a104:	0e8e                	slli	t4,t4,0x3
1c00a106:	00fe8733          	add	a4,t4,a5
1c00a10a:	01efef33          	or	t5,t6,t5
1c00a10e:	9f16                	add	t5,t5,t0
1c00a110:	01d732b3          	sltu	t0,a4,t4
1c00a114:	92fa                	add	t0,t0,t5
1c00a116:	01f75793          	srli	a5,a4,0x1f
1c00a11a:	0286                	slli	t0,t0,0x1
1c00a11c:	0057e2b3          	or	t0,a5,t0
1c00a120:	01c2d793          	srli	a5,t0,0x1c
1c00a124:	03078793          	addi	a5,a5,48
1c00a128:	ffd50813          	addi	a6,a0,-3
1c00a12c:	00f30123          	sb	a5,2(t1)
1c00a130:	cc42                	sw	a6,24(sp)
1c00a132:	00171793          	slli	a5,a4,0x1
1c00a136:	01c2f2b3          	and	t0,t0,t3
1c00a13a:	59005fe3          	blez	a6,1c00aed8 <pos_libc_prf+0x1ff2>
1c00a13e:	01e7df93          	srli	t6,a5,0x1e
1c00a142:	00229f13          	slli	t5,t0,0x2
1c00a146:	070e                	slli	a4,a4,0x3
1c00a148:	00f70eb3          	add	t4,a4,a5
1c00a14c:	01efef33          	or	t5,t6,t5
1c00a150:	9f16                	add	t5,t5,t0
1c00a152:	00eeb733          	sltu	a4,t4,a4
1c00a156:	977a                	add	a4,a4,t5
1c00a158:	01fed793          	srli	a5,t4,0x1f
1c00a15c:	0706                	slli	a4,a4,0x1
1c00a15e:	8f5d                	or	a4,a4,a5
1c00a160:	01c75793          	srli	a5,a4,0x1c
1c00a164:	03078793          	addi	a5,a5,48
1c00a168:	ffc50813          	addi	a6,a0,-4
1c00a16c:	00f301a3          	sb	a5,3(t1)
1c00a170:	cc42                	sw	a6,24(sp)
1c00a172:	001e9793          	slli	a5,t4,0x1
1c00a176:	01c772b3          	and	t0,a4,t3
1c00a17a:	55005ae3          	blez	a6,1c00aece <pos_libc_prf+0x1fe8>
1c00a17e:	01e7df93          	srli	t6,a5,0x1e
1c00a182:	00229f13          	slli	t5,t0,0x2
1c00a186:	0e8e                	slli	t4,t4,0x3
1c00a188:	00fe8733          	add	a4,t4,a5
1c00a18c:	01efef33          	or	t5,t6,t5
1c00a190:	9f16                	add	t5,t5,t0
1c00a192:	01d732b3          	sltu	t0,a4,t4
1c00a196:	92fa                	add	t0,t0,t5
1c00a198:	01f75793          	srli	a5,a4,0x1f
1c00a19c:	0286                	slli	t0,t0,0x1
1c00a19e:	0057e2b3          	or	t0,a5,t0
1c00a1a2:	01c2d793          	srli	a5,t0,0x1c
1c00a1a6:	03078793          	addi	a5,a5,48
1c00a1aa:	ffb50813          	addi	a6,a0,-5
1c00a1ae:	00f30223          	sb	a5,4(t1)
1c00a1b2:	cc42                	sw	a6,24(sp)
1c00a1b4:	00171793          	slli	a5,a4,0x1
1c00a1b8:	01c2f2b3          	and	t0,t0,t3
1c00a1bc:	510054e3          	blez	a6,1c00aec4 <pos_libc_prf+0x1fde>
1c00a1c0:	01e7df93          	srli	t6,a5,0x1e
1c00a1c4:	00229f13          	slli	t5,t0,0x2
1c00a1c8:	070e                	slli	a4,a4,0x3
1c00a1ca:	00f70eb3          	add	t4,a4,a5
1c00a1ce:	01efef33          	or	t5,t6,t5
1c00a1d2:	9f16                	add	t5,t5,t0
1c00a1d4:	00eeb733          	sltu	a4,t4,a4
1c00a1d8:	977a                	add	a4,a4,t5
1c00a1da:	01fed793          	srli	a5,t4,0x1f
1c00a1de:	0706                	slli	a4,a4,0x1
1c00a1e0:	8f5d                	or	a4,a4,a5
1c00a1e2:	01c75793          	srli	a5,a4,0x1c
1c00a1e6:	03078793          	addi	a5,a5,48
1c00a1ea:	ffa50813          	addi	a6,a0,-6
1c00a1ee:	00f302a3          	sb	a5,5(t1)
1c00a1f2:	cc42                	sw	a6,24(sp)
1c00a1f4:	001e9793          	slli	a5,t4,0x1
1c00a1f8:	01c772b3          	and	t0,a4,t3
1c00a1fc:	4b005fe3          	blez	a6,1c00aeba <pos_libc_prf+0x1fd4>
1c00a200:	01e7df93          	srli	t6,a5,0x1e
1c00a204:	00229f13          	slli	t5,t0,0x2
1c00a208:	0e8e                	slli	t4,t4,0x3
1c00a20a:	00fe8733          	add	a4,t4,a5
1c00a20e:	01efef33          	or	t5,t6,t5
1c00a212:	9f16                	add	t5,t5,t0
1c00a214:	01d732b3          	sltu	t0,a4,t4
1c00a218:	92fa                	add	t0,t0,t5
1c00a21a:	01f75793          	srli	a5,a4,0x1f
1c00a21e:	0286                	slli	t0,t0,0x1
1c00a220:	0057e2b3          	or	t0,a5,t0
1c00a224:	01c2d793          	srli	a5,t0,0x1c
1c00a228:	03078793          	addi	a5,a5,48
1c00a22c:	ff950813          	addi	a6,a0,-7
1c00a230:	00f30323          	sb	a5,6(t1)
1c00a234:	cc42                	sw	a6,24(sp)
1c00a236:	00171793          	slli	a5,a4,0x1
1c00a23a:	01c2f2b3          	and	t0,t0,t3
1c00a23e:	470059e3          	blez	a6,1c00aeb0 <pos_libc_prf+0x1fca>
1c00a242:	01e7df13          	srli	t5,a5,0x1e
1c00a246:	00229e93          	slli	t4,t0,0x2
1c00a24a:	070e                	slli	a4,a4,0x3
1c00a24c:	97ba                	add	a5,a5,a4
1c00a24e:	01df6eb3          	or	t4,t5,t4
1c00a252:	9e96                	add	t4,t4,t0
1c00a254:	00e7b733          	sltu	a4,a5,a4
1c00a258:	9776                	add	a4,a4,t4
1c00a25a:	0706                	slli	a4,a4,0x1
1c00a25c:	01f7de93          	srli	t4,a5,0x1f
1c00a260:	00eee733          	or	a4,t4,a4
1c00a264:	01c75e93          	srli	t4,a4,0x1c
1c00a268:	030e8e93          	addi	t4,t4,48
1c00a26c:	ff850813          	addi	a6,a0,-8
1c00a270:	0786                	slli	a5,a5,0x1
1c00a272:	01d303a3          	sb	t4,7(t1)
1c00a276:	cc42                	sw	a6,24(sp)
1c00a278:	8ebe                	mv	t4,a5
1c00a27a:	01c772b3          	and	t0,a4,t3
1c00a27e:	430054e3          	blez	a6,1c00aea6 <pos_libc_prf+0x1fc0>
1c00a282:	078a                	slli	a5,a5,0x2
1c00a284:	00229e13          	slli	t3,t0,0x2
1c00a288:	01eedf13          	srli	t5,t4,0x1e
1c00a28c:	01d78733          	add	a4,a5,t4
1c00a290:	01cf6e33          	or	t3,t5,t3
1c00a294:	9e16                	add	t3,t3,t0
1c00a296:	00f732b3          	sltu	t0,a4,a5
1c00a29a:	92f2                	add	t0,t0,t3
1c00a29c:	01f75793          	srli	a5,a4,0x1f
1c00a2a0:	0286                	slli	t0,t0,0x1
1c00a2a2:	0057e2b3          	or	t0,a5,t0
1c00a2a6:	01c2d793          	srli	a5,t0,0x1c
1c00a2aa:	03078793          	addi	a5,a5,48
1c00a2ae:	10000e37          	lui	t3,0x10000
1c00a2b2:	ff750813          	addi	a6,a0,-9
1c00a2b6:	1e7d                	addi	t3,t3,-1
1c00a2b8:	00f30423          	sb	a5,8(t1)
1c00a2bc:	cc42                	sw	a6,24(sp)
1c00a2be:	00171793          	slli	a5,a4,0x1
1c00a2c2:	01c2f2b3          	and	t0,t0,t3
1c00a2c6:	3b0059e3          	blez	a6,1c00ae78 <pos_libc_prf+0x1f92>
1c00a2ca:	01e7df93          	srli	t6,a5,0x1e
1c00a2ce:	00229f13          	slli	t5,t0,0x2
1c00a2d2:	070e                	slli	a4,a4,0x3
1c00a2d4:	00f70eb3          	add	t4,a4,a5
1c00a2d8:	01efef33          	or	t5,t6,t5
1c00a2dc:	9f16                	add	t5,t5,t0
1c00a2de:	00eeb733          	sltu	a4,t4,a4
1c00a2e2:	977a                	add	a4,a4,t5
1c00a2e4:	01fed793          	srli	a5,t4,0x1f
1c00a2e8:	0706                	slli	a4,a4,0x1
1c00a2ea:	8f5d                	or	a4,a4,a5
1c00a2ec:	01c75793          	srli	a5,a4,0x1c
1c00a2f0:	03078793          	addi	a5,a5,48
1c00a2f4:	ff650813          	addi	a6,a0,-10
1c00a2f8:	00f304a3          	sb	a5,9(t1)
1c00a2fc:	cc42                	sw	a6,24(sp)
1c00a2fe:	001e9793          	slli	a5,t4,0x1
1c00a302:	01c772b3          	and	t0,a4,t3
1c00a306:	370054e3          	blez	a6,1c00ae6e <pos_libc_prf+0x1f88>
1c00a30a:	01e7df93          	srli	t6,a5,0x1e
1c00a30e:	00229f13          	slli	t5,t0,0x2
1c00a312:	0e8e                	slli	t4,t4,0x3
1c00a314:	00fe8733          	add	a4,t4,a5
1c00a318:	01efef33          	or	t5,t6,t5
1c00a31c:	9f16                	add	t5,t5,t0
1c00a31e:	01d732b3          	sltu	t0,a4,t4
1c00a322:	92fa                	add	t0,t0,t5
1c00a324:	01f75793          	srli	a5,a4,0x1f
1c00a328:	0286                	slli	t0,t0,0x1
1c00a32a:	0057e2b3          	or	t0,a5,t0
1c00a32e:	01c2d793          	srli	a5,t0,0x1c
1c00a332:	03078793          	addi	a5,a5,48
1c00a336:	ff550813          	addi	a6,a0,-11
1c00a33a:	00f30523          	sb	a5,10(t1)
1c00a33e:	cc42                	sw	a6,24(sp)
1c00a340:	00171793          	slli	a5,a4,0x1
1c00a344:	01c2f2b3          	and	t0,t0,t3
1c00a348:	31005ee3          	blez	a6,1c00ae64 <pos_libc_prf+0x1f7e>
1c00a34c:	01e7df93          	srli	t6,a5,0x1e
1c00a350:	00229f13          	slli	t5,t0,0x2
1c00a354:	070e                	slli	a4,a4,0x3
1c00a356:	00f70eb3          	add	t4,a4,a5
1c00a35a:	01efef33          	or	t5,t6,t5
1c00a35e:	9f16                	add	t5,t5,t0
1c00a360:	00eeb733          	sltu	a4,t4,a4
1c00a364:	977a                	add	a4,a4,t5
1c00a366:	01fed793          	srli	a5,t4,0x1f
1c00a36a:	0706                	slli	a4,a4,0x1
1c00a36c:	8f5d                	or	a4,a4,a5
1c00a36e:	01c75793          	srli	a5,a4,0x1c
1c00a372:	03078793          	addi	a5,a5,48
1c00a376:	ff450813          	addi	a6,a0,-12
1c00a37a:	00f305a3          	sb	a5,11(t1)
1c00a37e:	cc42                	sw	a6,24(sp)
1c00a380:	01c772b3          	and	t0,a4,t3
1c00a384:	001e9793          	slli	a5,t4,0x1
1c00a388:	00c30713          	addi	a4,t1,12
1c00a38c:	11005563          	blez	a6,1c00a496 <pos_libc_prf+0x15b0>
1c00a390:	003e9613          	slli	a2,t4,0x3
1c00a394:	01e7df93          	srli	t6,a5,0x1e
1c00a398:	00229f13          	slli	t5,t0,0x2
1c00a39c:	00f60733          	add	a4,a2,a5
1c00a3a0:	01efef33          	or	t5,t6,t5
1c00a3a4:	9f16                	add	t5,t5,t0
1c00a3a6:	00c732b3          	sltu	t0,a4,a2
1c00a3aa:	92fa                	add	t0,t0,t5
1c00a3ac:	01f75793          	srli	a5,a4,0x1f
1c00a3b0:	0286                	slli	t0,t0,0x1
1c00a3b2:	0057e2b3          	or	t0,a5,t0
1c00a3b6:	01c2d793          	srli	a5,t0,0x1c
1c00a3ba:	03078793          	addi	a5,a5,48
1c00a3be:	ff350613          	addi	a2,a0,-13
1c00a3c2:	00f30623          	sb	a5,12(t1)
1c00a3c6:	cc32                	sw	a2,24(sp)
1c00a3c8:	00171793          	slli	a5,a4,0x1
1c00a3cc:	01c2f2b3          	and	t0,t0,t3
1c00a3d0:	2cc056e3          	blez	a2,1c00ae9c <pos_libc_prf+0x1fb6>
1c00a3d4:	01e7df13          	srli	t5,a5,0x1e
1c00a3d8:	00229e93          	slli	t4,t0,0x2
1c00a3dc:	070e                	slli	a4,a4,0x3
1c00a3de:	00f70633          	add	a2,a4,a5
1c00a3e2:	01df6eb3          	or	t4,t5,t4
1c00a3e6:	9e96                	add	t4,t4,t0
1c00a3e8:	00e63733          	sltu	a4,a2,a4
1c00a3ec:	9776                	add	a4,a4,t4
1c00a3ee:	01f65793          	srli	a5,a2,0x1f
1c00a3f2:	0706                	slli	a4,a4,0x1
1c00a3f4:	8f5d                	or	a4,a4,a5
1c00a3f6:	01c75793          	srli	a5,a4,0x1c
1c00a3fa:	03078793          	addi	a5,a5,48
1c00a3fe:	ff250813          	addi	a6,a0,-14
1c00a402:	00f306a3          	sb	a5,13(t1)
1c00a406:	cc42                	sw	a6,24(sp)
1c00a408:	00161793          	slli	a5,a2,0x1
1c00a40c:	01c772b3          	and	t0,a4,t3
1c00a410:	290051e3          	blez	a6,1c00ae92 <pos_libc_prf+0x1fac>
1c00a414:	060e                	slli	a2,a2,0x3
1c00a416:	01e7df13          	srli	t5,a5,0x1e
1c00a41a:	00229e93          	slli	t4,t0,0x2
1c00a41e:	00f60733          	add	a4,a2,a5
1c00a422:	01df6eb3          	or	t4,t5,t4
1c00a426:	9e96                	add	t4,t4,t0
1c00a428:	00c732b3          	sltu	t0,a4,a2
1c00a42c:	92f6                	add	t0,t0,t4
1c00a42e:	01f75793          	srli	a5,a4,0x1f
1c00a432:	0286                	slli	t0,t0,0x1
1c00a434:	0057e2b3          	or	t0,a5,t0
1c00a438:	01c2d793          	srli	a5,t0,0x1c
1c00a43c:	03078793          	addi	a5,a5,48
1c00a440:	ff150613          	addi	a2,a0,-15
1c00a444:	00f30723          	sb	a5,14(t1)
1c00a448:	cc32                	sw	a2,24(sp)
1c00a44a:	00171793          	slli	a5,a4,0x1
1c00a44e:	01c2f2b3          	and	t0,t0,t3
1c00a452:	20c054e3          	blez	a2,1c00ae5a <pos_libc_prf+0x1f74>
1c00a456:	070e                	slli	a4,a4,0x3
1c00a458:	01e7de93          	srli	t4,a5,0x1e
1c00a45c:	00229613          	slli	a2,t0,0x2
1c00a460:	97ba                	add	a5,a5,a4
1c00a462:	00cee633          	or	a2,t4,a2
1c00a466:	9616                	add	a2,a2,t0
1c00a468:	00e7b2b3          	sltu	t0,a5,a4
1c00a46c:	92b2                	add	t0,t0,a2
1c00a46e:	01f7d713          	srli	a4,a5,0x1f
1c00a472:	0286                	slli	t0,t0,0x1
1c00a474:	005762b3          	or	t0,a4,t0
1c00a478:	01c2d613          	srli	a2,t0,0x1c
1c00a47c:	03060613          	addi	a2,a2,48
1c00a480:	00c307a3          	sb	a2,15(t1)
1c00a484:	ff050613          	addi	a2,a0,-16
1c00a488:	cc32                	sw	a2,24(sp)
1c00a48a:	01030713          	addi	a4,t1,16
1c00a48e:	0786                	slli	a5,a5,0x1
1c00a490:	01c2f2b3          	and	t0,t0,t3
1c00a494:	4601                	li	a2,0
1c00a496:	5532                	lw	a0,44(sp)
1c00a498:	080513e3          	bnez	a0,1c00ad1e <pos_libc_prf+0x1e38>
1c00a49c:	10040ee3          	beqz	s0,1c00adb8 <pos_libc_prf+0x1ed2>
1c00a4a0:	833a                	mv	t1,a4
1c00a4a2:	02e00713          	li	a4,46
1c00a4a6:	00e300ab          	p.sb	a4,1(t1!)
1c00a4aa:	4ee2                	lw	t4,24(sp)
1c00a4ac:	8e22                	mv	t3,s0
1c00a4ae:	4501                	li	a0,0
1c00a4b0:	d402                	sw	zero,40(sp)
1c00a4b2:	060601e3          	beqz	a2,1c00ad14 <pos_libc_prf+0x1e2e>
1c00a4b6:	00279713          	slli	a4,a5,0x2
1c00a4ba:	01e7df93          	srli	t6,a5,0x1e
1c00a4be:	00229f13          	slli	t5,t0,0x2
1c00a4c2:	97ba                	add	a5,a5,a4
1c00a4c4:	01efef33          	or	t5,t6,t5
1c00a4c8:	00e7bfb3          	sltu	t6,a5,a4
1c00a4cc:	005f0733          	add	a4,t5,t0
1c00a4d0:	9fba                	add	t6,t6,a4
1c00a4d2:	0f86                	slli	t6,t6,0x1
1c00a4d4:	01f7d713          	srli	a4,a5,0x1f
1c00a4d8:	01f76fb3          	or	t6,a4,t6
1c00a4dc:	01cfd293          	srli	t0,t6,0x1c
1c00a4e0:	03028293          	addi	t0,t0,48 # 80000030 <__l2_shared_end+0x63fefc28>
1c00a4e4:	871a                	mv	a4,t1
1c00a4e6:	10000f37          	lui	t5,0x10000
1c00a4ea:	1f7d                	addi	t5,t5,-1
1c00a4ec:	005700ab          	p.sb	t0,1(a4!)
1c00a4f0:	fff60393          	addi	t2,a2,-1
1c00a4f4:	00179293          	slli	t0,a5,0x1
1c00a4f8:	01efffb3          	and	t6,t6,t5
1c00a4fc:	fffe0413          	addi	s0,t3,-1 # fffffff <__l1_heap_size+0xffc001f>
1c00a500:	441e2863          	p.beqimm	t3,1,1c00a950 <pos_libc_prf+0x1a6a>
1c00a504:	44038663          	beqz	t2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a508:	01e2d393          	srli	t2,t0,0x1e
1c00a50c:	078e                	slli	a5,a5,0x3
1c00a50e:	002f9713          	slli	a4,t6,0x2
1c00a512:	92be                	add	t0,t0,a5
1c00a514:	00e3e733          	or	a4,t2,a4
1c00a518:	9fba                	add	t6,t6,a4
1c00a51a:	00f2b7b3          	sltu	a5,t0,a5
1c00a51e:	97fe                	add	a5,a5,t6
1c00a520:	01f2d713          	srli	a4,t0,0x1f
1c00a524:	0786                	slli	a5,a5,0x1
1c00a526:	8fd9                	or	a5,a5,a4
1c00a528:	01c7d713          	srli	a4,a5,0x1c
1c00a52c:	03070713          	addi	a4,a4,48
1c00a530:	00e300a3          	sb	a4,1(t1)
1c00a534:	ffe60913          	addi	s2,a2,-2
1c00a538:	00230713          	addi	a4,t1,2
1c00a53c:	00129f93          	slli	t6,t0,0x1
1c00a540:	01e7f3b3          	and	t2,a5,t5
1c00a544:	ffee0413          	addi	s0,t3,-2
1c00a548:	402e2463          	p.beqimm	t3,2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a54c:	40090263          	beqz	s2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a550:	00329713          	slli	a4,t0,0x3
1c00a554:	01efd793          	srli	a5,t6,0x1e
1c00a558:	00239293          	slli	t0,t2,0x2
1c00a55c:	9fba                	add	t6,t6,a4
1c00a55e:	0057e2b3          	or	t0,a5,t0
1c00a562:	929e                	add	t0,t0,t2
1c00a564:	00efb7b3          	sltu	a5,t6,a4
1c00a568:	9796                	add	a5,a5,t0
1c00a56a:	01ffd713          	srli	a4,t6,0x1f
1c00a56e:	0786                	slli	a5,a5,0x1
1c00a570:	8fd9                	or	a5,a5,a4
1c00a572:	01c7d713          	srli	a4,a5,0x1c
1c00a576:	03070713          	addi	a4,a4,48
1c00a57a:	00e30123          	sb	a4,2(t1)
1c00a57e:	ffd60913          	addi	s2,a2,-3
1c00a582:	00330713          	addi	a4,t1,3
1c00a586:	001f9293          	slli	t0,t6,0x1
1c00a58a:	01e7f3b3          	and	t2,a5,t5
1c00a58e:	ffde0413          	addi	s0,t3,-3
1c00a592:	3a3e2f63          	p.beqimm	t3,3,1c00a950 <pos_libc_prf+0x1a6a>
1c00a596:	3a090d63          	beqz	s2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a59a:	0f8e                	slli	t6,t6,0x3
1c00a59c:	01e2d793          	srli	a5,t0,0x1e
1c00a5a0:	00239713          	slli	a4,t2,0x2
1c00a5a4:	92fe                	add	t0,t0,t6
1c00a5a6:	8f5d                	or	a4,a4,a5
1c00a5a8:	971e                	add	a4,a4,t2
1c00a5aa:	01f2b7b3          	sltu	a5,t0,t6
1c00a5ae:	97ba                	add	a5,a5,a4
1c00a5b0:	0786                	slli	a5,a5,0x1
1c00a5b2:	01f2d713          	srli	a4,t0,0x1f
1c00a5b6:	8fd9                	or	a5,a5,a4
1c00a5b8:	01c7d713          	srli	a4,a5,0x1c
1c00a5bc:	03070713          	addi	a4,a4,48
1c00a5c0:	00e301a3          	sb	a4,3(t1)
1c00a5c4:	ffc60913          	addi	s2,a2,-4
1c00a5c8:	00430713          	addi	a4,t1,4
1c00a5cc:	00129393          	slli	t2,t0,0x1
1c00a5d0:	01e7ffb3          	and	t6,a5,t5
1c00a5d4:	ffce0413          	addi	s0,t3,-4
1c00a5d8:	364e2c63          	p.beqimm	t3,4,1c00a950 <pos_libc_prf+0x1a6a>
1c00a5dc:	36090a63          	beqz	s2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a5e0:	00329713          	slli	a4,t0,0x3
1c00a5e4:	01e3d793          	srli	a5,t2,0x1e
1c00a5e8:	002f9413          	slli	s0,t6,0x2
1c00a5ec:	007702b3          	add	t0,a4,t2
1c00a5f0:	8c5d                	or	s0,s0,a5
1c00a5f2:	9fa2                	add	t6,t6,s0
1c00a5f4:	00e2b7b3          	sltu	a5,t0,a4
1c00a5f8:	97fe                	add	a5,a5,t6
1c00a5fa:	01f2d713          	srli	a4,t0,0x1f
1c00a5fe:	0786                	slli	a5,a5,0x1
1c00a600:	8fd9                	or	a5,a5,a4
1c00a602:	01c7d713          	srli	a4,a5,0x1c
1c00a606:	03070713          	addi	a4,a4,48
1c00a60a:	00e30223          	sb	a4,4(t1)
1c00a60e:	ffb60393          	addi	t2,a2,-5
1c00a612:	00530713          	addi	a4,t1,5
1c00a616:	00129f93          	slli	t6,t0,0x1
1c00a61a:	01e7f7b3          	and	a5,a5,t5
1c00a61e:	ffbe0413          	addi	s0,t3,-5
1c00a622:	325e2763          	p.beqimm	t3,5,1c00a950 <pos_libc_prf+0x1a6a>
1c00a626:	32038563          	beqz	t2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a62a:	01efd713          	srli	a4,t6,0x1e
1c00a62e:	002f9f13          	slli	t5,t6,0x2
1c00a632:	00279393          	slli	t2,a5,0x2
1c00a636:	9ffa                	add	t6,t6,t5
1c00a638:	007763b3          	or	t2,a4,t2
1c00a63c:	01efbf33          	sltu	t5,t6,t5
1c00a640:	979e                	add	a5,a5,t2
1c00a642:	97fa                	add	a5,a5,t5
1c00a644:	01ffd713          	srli	a4,t6,0x1f
1c00a648:	0786                	slli	a5,a5,0x1
1c00a64a:	8fd9                	or	a5,a5,a4
1c00a64c:	01c7d713          	srli	a4,a5,0x1c
1c00a650:	03070713          	addi	a4,a4,48
1c00a654:	10000f37          	lui	t5,0x10000
1c00a658:	1f7d                	addi	t5,t5,-1
1c00a65a:	00e302a3          	sb	a4,5(t1)
1c00a65e:	ffa60913          	addi	s2,a2,-6
1c00a662:	00630713          	addi	a4,t1,6
1c00a666:	001f9393          	slli	t2,t6,0x1
1c00a66a:	01e7f2b3          	and	t0,a5,t5
1c00a66e:	ffae0413          	addi	s0,t3,-6
1c00a672:	2c6e2f63          	p.beqimm	t3,6,1c00a950 <pos_libc_prf+0x1a6a>
1c00a676:	2c090d63          	beqz	s2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a67a:	003f9713          	slli	a4,t6,0x3
1c00a67e:	01e3d413          	srli	s0,t2,0x1e
1c00a682:	00229f93          	slli	t6,t0,0x2
1c00a686:	007707b3          	add	a5,a4,t2
1c00a68a:	01f46fb3          	or	t6,s0,t6
1c00a68e:	9f96                	add	t6,t6,t0
1c00a690:	00e7b733          	sltu	a4,a5,a4
1c00a694:	977e                	add	a4,a4,t6
1c00a696:	01f7d293          	srli	t0,a5,0x1f
1c00a69a:	00171f93          	slli	t6,a4,0x1
1c00a69e:	01f2efb3          	or	t6,t0,t6
1c00a6a2:	01cfd713          	srli	a4,t6,0x1c
1c00a6a6:	03070713          	addi	a4,a4,48
1c00a6aa:	00e30323          	sb	a4,6(t1)
1c00a6ae:	ff960293          	addi	t0,a2,-7
1c00a6b2:	00730713          	addi	a4,t1,7
1c00a6b6:	00179393          	slli	t2,a5,0x1
1c00a6ba:	01efffb3          	and	t6,t6,t5
1c00a6be:	ff9e0413          	addi	s0,t3,-7
1c00a6c2:	287e2763          	p.beqimm	t3,7,1c00a950 <pos_libc_prf+0x1a6a>
1c00a6c6:	28028563          	beqz	t0,1c00a950 <pos_libc_prf+0x1a6a>
1c00a6ca:	01e3d293          	srli	t0,t2,0x1e
1c00a6ce:	078e                	slli	a5,a5,0x3
1c00a6d0:	002f9713          	slli	a4,t6,0x2
1c00a6d4:	93be                	add	t2,t2,a5
1c00a6d6:	00e2e733          	or	a4,t0,a4
1c00a6da:	9fba                	add	t6,t6,a4
1c00a6dc:	00f3b7b3          	sltu	a5,t2,a5
1c00a6e0:	97fe                	add	a5,a5,t6
1c00a6e2:	01f3d713          	srli	a4,t2,0x1f
1c00a6e6:	0786                	slli	a5,a5,0x1
1c00a6e8:	8fd9                	or	a5,a5,a4
1c00a6ea:	01c7d713          	srli	a4,a5,0x1c
1c00a6ee:	03070713          	addi	a4,a4,48
1c00a6f2:	00e303a3          	sb	a4,7(t1)
1c00a6f6:	ff860f93          	addi	t6,a2,-8
1c00a6fa:	00830713          	addi	a4,t1,8
1c00a6fe:	00139293          	slli	t0,t2,0x1
1c00a702:	01e7f7b3          	and	a5,a5,t5
1c00a706:	ff8e0413          	addi	s0,t3,-8
1c00a70a:	248e2363          	p.beqimm	t3,8,1c00a950 <pos_libc_prf+0x1a6a>
1c00a70e:	240f8163          	beqz	t6,1c00a950 <pos_libc_prf+0x1a6a>
1c00a712:	00339713          	slli	a4,t2,0x3
1c00a716:	00279f93          	slli	t6,a5,0x2
1c00a71a:	01e2d393          	srli	t2,t0,0x1e
1c00a71e:	01f3efb3          	or	t6,t2,t6
1c00a722:	92ba                	add	t0,t0,a4
1c00a724:	97fe                	add	a5,a5,t6
1c00a726:	00e2b733          	sltu	a4,t0,a4
1c00a72a:	973e                	add	a4,a4,a5
1c00a72c:	00171f93          	slli	t6,a4,0x1
1c00a730:	01f2d793          	srli	a5,t0,0x1f
1c00a734:	01f7efb3          	or	t6,a5,t6
1c00a738:	01cfd793          	srli	a5,t6,0x1c
1c00a73c:	03078793          	addi	a5,a5,48
1c00a740:	00f30423          	sb	a5,8(t1)
1c00a744:	00930713          	addi	a4,t1,9
1c00a748:	ff760793          	addi	a5,a2,-9
1c00a74c:	00129393          	slli	t2,t0,0x1
1c00a750:	01efffb3          	and	t6,t6,t5
1c00a754:	ff7e0413          	addi	s0,t3,-9
1c00a758:	1e9e2c63          	p.beqimm	t3,9,1c00a950 <pos_libc_prf+0x1a6a>
1c00a75c:	1e078a63          	beqz	a5,1c00a950 <pos_libc_prf+0x1a6a>
1c00a760:	01e3d413          	srli	s0,t2,0x1e
1c00a764:	00329793          	slli	a5,t0,0x3
1c00a768:	002f9713          	slli	a4,t6,0x2
1c00a76c:	007782b3          	add	t0,a5,t2
1c00a770:	8f41                	or	a4,a4,s0
1c00a772:	9fba                	add	t6,t6,a4
1c00a774:	00f2b7b3          	sltu	a5,t0,a5
1c00a778:	97fe                	add	a5,a5,t6
1c00a77a:	01f2d713          	srli	a4,t0,0x1f
1c00a77e:	0786                	slli	a5,a5,0x1
1c00a780:	8fd9                	or	a5,a5,a4
1c00a782:	01c7d713          	srli	a4,a5,0x1c
1c00a786:	03070713          	addi	a4,a4,48
1c00a78a:	00e304a3          	sb	a4,9(t1)
1c00a78e:	ff660393          	addi	t2,a2,-10
1c00a792:	00a30713          	addi	a4,t1,10
1c00a796:	00129f93          	slli	t6,t0,0x1
1c00a79a:	01e7f7b3          	and	a5,a5,t5
1c00a79e:	ff6e0413          	addi	s0,t3,-10
1c00a7a2:	1aae2763          	p.beqimm	t3,10,1c00a950 <pos_libc_prf+0x1a6a>
1c00a7a6:	1a038563          	beqz	t2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a7aa:	01efd713          	srli	a4,t6,0x1e
1c00a7ae:	002f9f13          	slli	t5,t6,0x2
1c00a7b2:	00279393          	slli	t2,a5,0x2
1c00a7b6:	9ffa                	add	t6,t6,t5
1c00a7b8:	007763b3          	or	t2,a4,t2
1c00a7bc:	01efbf33          	sltu	t5,t6,t5
1c00a7c0:	979e                	add	a5,a5,t2
1c00a7c2:	97fa                	add	a5,a5,t5
1c00a7c4:	01ffd713          	srli	a4,t6,0x1f
1c00a7c8:	0786                	slli	a5,a5,0x1
1c00a7ca:	8fd9                	or	a5,a5,a4
1c00a7cc:	01c7d713          	srli	a4,a5,0x1c
1c00a7d0:	03070713          	addi	a4,a4,48
1c00a7d4:	10000f37          	lui	t5,0x10000
1c00a7d8:	1f7d                	addi	t5,t5,-1
1c00a7da:	00e30523          	sb	a4,10(t1)
1c00a7de:	ff560913          	addi	s2,a2,-11
1c00a7e2:	00b30713          	addi	a4,t1,11
1c00a7e6:	001f9393          	slli	t2,t6,0x1
1c00a7ea:	01e7f2b3          	and	t0,a5,t5
1c00a7ee:	ff5e0413          	addi	s0,t3,-11
1c00a7f2:	14be2f63          	p.beqimm	t3,11,1c00a950 <pos_libc_prf+0x1a6a>
1c00a7f6:	14090d63          	beqz	s2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a7fa:	003f9713          	slli	a4,t6,0x3
1c00a7fe:	01e3d413          	srli	s0,t2,0x1e
1c00a802:	00229f93          	slli	t6,t0,0x2
1c00a806:	007707b3          	add	a5,a4,t2
1c00a80a:	01f46fb3          	or	t6,s0,t6
1c00a80e:	9f96                	add	t6,t6,t0
1c00a810:	00e7b733          	sltu	a4,a5,a4
1c00a814:	977e                	add	a4,a4,t6
1c00a816:	01f7d293          	srli	t0,a5,0x1f
1c00a81a:	00171f93          	slli	t6,a4,0x1
1c00a81e:	01f2efb3          	or	t6,t0,t6
1c00a822:	01cfd713          	srli	a4,t6,0x1c
1c00a826:	03070713          	addi	a4,a4,48
1c00a82a:	00e305a3          	sb	a4,11(t1)
1c00a82e:	ff460393          	addi	t2,a2,-12
1c00a832:	00c30713          	addi	a4,t1,12
1c00a836:	00179293          	slli	t0,a5,0x1
1c00a83a:	01efffb3          	and	t6,t6,t5
1c00a83e:	ff4e0413          	addi	s0,t3,-12
1c00a842:	10ce2763          	p.beqimm	t3,12,1c00a950 <pos_libc_prf+0x1a6a>
1c00a846:	10038563          	beqz	t2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a84a:	01e2d393          	srli	t2,t0,0x1e
1c00a84e:	078e                	slli	a5,a5,0x3
1c00a850:	002f9713          	slli	a4,t6,0x2
1c00a854:	92be                	add	t0,t0,a5
1c00a856:	00e3e733          	or	a4,t2,a4
1c00a85a:	9fba                	add	t6,t6,a4
1c00a85c:	00f2b7b3          	sltu	a5,t0,a5
1c00a860:	97fe                	add	a5,a5,t6
1c00a862:	01f2d713          	srli	a4,t0,0x1f
1c00a866:	00179f93          	slli	t6,a5,0x1
1c00a86a:	01f76fb3          	or	t6,a4,t6
1c00a86e:	01cfd793          	srli	a5,t6,0x1c
1c00a872:	03078793          	addi	a5,a5,48
1c00a876:	00f30623          	sb	a5,12(t1)
1c00a87a:	00d30713          	addi	a4,t1,13
1c00a87e:	ff360393          	addi	t2,a2,-13
1c00a882:	00129793          	slli	a5,t0,0x1
1c00a886:	01efffb3          	and	t6,t6,t5
1c00a88a:	ff3e0413          	addi	s0,t3,-13
1c00a88e:	0cde2163          	p.beqimm	t3,13,1c00a950 <pos_libc_prf+0x1a6a>
1c00a892:	0a038f63          	beqz	t2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a896:	00329713          	slli	a4,t0,0x3
1c00a89a:	01e7d393          	srli	t2,a5,0x1e
1c00a89e:	002f9293          	slli	t0,t6,0x2
1c00a8a2:	97ba                	add	a5,a5,a4
1c00a8a4:	0053e2b3          	or	t0,t2,t0
1c00a8a8:	9f96                	add	t6,t6,t0
1c00a8aa:	00e7b733          	sltu	a4,a5,a4
1c00a8ae:	977e                	add	a4,a4,t6
1c00a8b0:	01f7d293          	srli	t0,a5,0x1f
1c00a8b4:	00171f93          	slli	t6,a4,0x1
1c00a8b8:	01f2efb3          	or	t6,t0,t6
1c00a8bc:	01cfd713          	srli	a4,t6,0x1c
1c00a8c0:	03070713          	addi	a4,a4,48
1c00a8c4:	00e306a3          	sb	a4,13(t1)
1c00a8c8:	1649                	addi	a2,a2,-14
1c00a8ca:	00e30713          	addi	a4,t1,14
1c00a8ce:	00179393          	slli	t2,a5,0x1
1c00a8d2:	01eff2b3          	and	t0,t6,t5
1c00a8d6:	ff2e0413          	addi	s0,t3,-14
1c00a8da:	06ee2b63          	p.beqimm	t3,14,1c00a950 <pos_libc_prf+0x1a6a>
1c00a8de:	ca2d                	beqz	a2,1c00a950 <pos_libc_prf+0x1a6a>
1c00a8e0:	078e                	slli	a5,a5,0x3
1c00a8e2:	01e3d413          	srli	s0,t2,0x1e
1c00a8e6:	00229713          	slli	a4,t0,0x2
1c00a8ea:	00778fb3          	add	t6,a5,t2
1c00a8ee:	8f41                	or	a4,a4,s0
1c00a8f0:	00ffb7b3          	sltu	a5,t6,a5
1c00a8f4:	9716                	add	a4,a4,t0
1c00a8f6:	97ba                	add	a5,a5,a4
1c00a8f8:	0786                	slli	a5,a5,0x1
1c00a8fa:	01ffd713          	srli	a4,t6,0x1f
1c00a8fe:	8fd9                	or	a5,a5,a4
1c00a900:	01c7d713          	srli	a4,a5,0x1c
1c00a904:	03070713          	addi	a4,a4,48
1c00a908:	00e30723          	sb	a4,14(t1)
1c00a90c:	0f86                	slli	t6,t6,0x1
1c00a90e:	00f30713          	addi	a4,t1,15
1c00a912:	01e7ff33          	and	t5,a5,t5
1c00a916:	ff1e0413          	addi	s0,t3,-15
1c00a91a:	02fe2b63          	p.beqimm	t3,15,1c00a950 <pos_libc_prf+0x1a6a>
1c00a91e:	02162963          	p.beqimm	a2,1,1c00a950 <pos_libc_prf+0x1a6a>
1c00a922:	002f9613          	slli	a2,t6,0x2
1c00a926:	01efd793          	srli	a5,t6,0x1e
1c00a92a:	002f1713          	slli	a4,t5,0x2
1c00a92e:	8f5d                	or	a4,a4,a5
1c00a930:	01f607b3          	add	a5,a2,t6
1c00a934:	977a                	add	a4,a4,t5
1c00a936:	00c7b7b3          	sltu	a5,a5,a2
1c00a93a:	97ba                	add	a5,a5,a4
1c00a93c:	0786                	slli	a5,a5,0x1
1c00a93e:	83f1                	srli	a5,a5,0x1c
1c00a940:	03078793          	addi	a5,a5,48
1c00a944:	01030713          	addi	a4,t1,16
1c00a948:	00f307a3          	sb	a5,15(t1)
1c00a94c:	ff0e0413          	addi	s0,t3,-16
1c00a950:	a2059663          	bnez	a1,1c009b7c <pos_libc_prf+0xc96>
1c00a954:	9ea2                	add	t4,t4,s0
1c00a956:	a4eff06f          	j	1c009ba4 <pos_libc_prf+0xcbe>
1c00a95a:	8ea2                	mv	t4,s0
1c00a95c:	d402                	sw	zero,40(sp)
1c00a95e:	cc02                	sw	zero,24(sp)
1c00a960:	a38ff06f          	j	1c009b98 <pos_libc_prf+0xcb2>
1c00a964:	47e2                	lw	a5,24(sp)
1c00a966:	00778713          	addi	a4,a5,7
1c00a96a:	c4073733          	p.bclr	a4,a4,2,0
1c00a96e:	8d3a                	mv	s10,a4
1c00a970:	4358                	lw	a4,4(a4)
1c00a972:	008d278b          	p.lw	a5,8(s10!)
1c00a976:	d43a                	sw	a4,40(sp)
1c00a978:	86ba                	mv	a3,a4
1c00a97a:	871fe06f          	j	1c0091ea <pos_libc_prf+0x304>
1c00a97e:	47e2                	lw	a5,24(sp)
1c00a980:	079d                	addi	a5,a5,7
1c00a982:	c407b7b3          	p.bclr	a5,a5,2,0
1c00a986:	8d3e                	mv	s10,a5
1c00a988:	008d280b          	p.lw	a6,8(s10!)
1c00a98c:	0047aa83          	lw	s5,4(a5)
1c00a990:	a0ffe06f          	j	1c00939e <pos_libc_prf+0x4b8>
1c00a994:	4e81                	li	t4,0
1c00a996:	4581                	li	a1,0
1c00a998:	cc3fe06f          	j	1c00965a <pos_libc_prf+0x774>
1c00a99c:	c822                	sw	s0,16(sp)
1c00a99e:	03410913          	addi	s2,sp,52
1c00a9a2:	8456                	mv	s0,s5
1c00a9a4:	4ba5                	li	s7,9
1c00a9a6:	8ac2                	mv	s5,a6
1c00a9a8:	4629                	li	a2,10
1c00a9aa:	4681                	li	a3,0
1c00a9ac:	8556                	mv	a0,s5
1c00a9ae:	85a2                	mv	a1,s0
1c00a9b0:	a2dfd0ef          	jal	ra,1c0083dc <__umoddi3>
1c00a9b4:	89aa                	mv	s3,a0
1c00a9b6:	85a2                	mv	a1,s0
1c00a9b8:	8556                	mv	a0,s5
1c00a9ba:	4629                	li	a2,10
1c00a9bc:	4681                	li	a3,0
1c00a9be:	edefd0ef          	jal	ra,1c00809c <__udivdi3>
1c00a9c2:	03098993          	addi	s3,s3,48
1c00a9c6:	874a                	mv	a4,s2
1c00a9c8:	86a2                	mv	a3,s0
1c00a9ca:	013900ab          	p.sb	s3,1(s2!)
1c00a9ce:	8656                	mv	a2,s5
1c00a9d0:	842e                	mv	s0,a1
1c00a9d2:	8aaa                	mv	s5,a0
1c00a9d4:	faf1                	bnez	a3,1c00a9a8 <pos_libc_prf+0x1ac2>
1c00a9d6:	fccbe9e3          	bltu	s7,a2,1c00a9a8 <pos_libc_prf+0x1ac2>
1c00a9da:	185c                	addi	a5,sp,52
1c00a9dc:	00090023          	sb	zero,0(s2)
1c00a9e0:	4442                	lw	s0,16(sp)
1c00a9e2:	40f909b3          	sub	s3,s2,a5
1c00a9e6:	0ae7f763          	bgeu	a5,a4,1c00aa94 <pos_libc_prf+0x1bae>
1c00a9ea:	5792                	lw	a5,36(sp)
1c00a9ec:	1854                	addi	a3,sp,52
1c00a9ee:	00e78633          	add	a2,a5,a4
1c00a9f2:	00165793          	srli	a5,a2,0x1
1c00a9f6:	00178313          	addi	t1,a5,1
1c00a9fa:	969a                	add	a3,a3,t1
1c00a9fc:	40f707b3          	sub	a5,a4,a5
1c00aa00:	04f6b6b3          	p.sletu	a3,a3,a5
1c00aa04:	185c                	addi	a5,sp,52
1c00aa06:	04f937b3          	p.sletu	a5,s2,a5
1c00aa0a:	8fd5                	or	a5,a5,a3
1c00aa0c:	86ba                	mv	a3,a4
1c00aa0e:	34078463          	beqz	a5,1c00ad56 <pos_libc_prf+0x1e70>
1c00aa12:	00663613          	sltiu	a2,a2,6
1c00aa16:	34061063          	bnez	a2,1c00ad56 <pos_libc_prf+0x1e70>
1c00aa1a:	c2033633          	p.bclr	a2,t1,1,0
1c00aa1e:	1671                	addi	a2,a2,-4
1c00aa20:	8209                	srli	a2,a2,0x2
1c00aa22:	ffd70513          	addi	a0,a4,-3
1c00aa26:	184c                	addi	a1,sp,52
1c00aa28:	0605                	addi	a2,a2,1
1c00aa2a:	00a640fb          	lp.setup	x1,a2,1c00aa3e <pos_libc_prf+0x1b58>
1c00aa2e:	4194                	lw	a3,0(a1)
1c00aa30:	411c                	lw	a5,0(a0)
1c00aa32:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c00aa36:	fed52e2b          	p.sw	a3,-4(a0!)
1c00aa3a:	c2d7f7d7          	pv.shufflei0.sci.b	a5,a5,27
1c00aa3e:	00f5a22b          	p.sw	a5,4(a1!)
1c00aa42:	c20336b3          	p.bclr	a3,t1,1,0
1c00aa46:	185c                	addi	a5,sp,52
1c00aa48:	8f15                	sub	a4,a4,a3
1c00aa4a:	97b6                	add	a5,a5,a3
1c00aa4c:	04668463          	beq	a3,t1,1c00aa94 <pos_libc_prf+0x1bae>
1c00aa50:	0007c603          	lbu	a2,0(a5)
1c00aa54:	00074583          	lbu	a1,0(a4)
1c00aa58:	86ba                	mv	a3,a4
1c00aa5a:	fec68fab          	p.sb	a2,-1(a3!)
1c00aa5e:	863e                	mv	a2,a5
1c00aa60:	00b600ab          	p.sb	a1,1(a2!)
1c00aa64:	02d67863          	bgeu	a2,a3,1c00aa94 <pos_libc_prf+0x1bae>
1c00aa68:	0017c583          	lbu	a1,1(a5)
1c00aa6c:	fff74603          	lbu	a2,-1(a4)
1c00aa70:	00278693          	addi	a3,a5,2
1c00aa74:	feb70fa3          	sb	a1,-1(a4)
1c00aa78:	00c780a3          	sb	a2,1(a5)
1c00aa7c:	ffe70613          	addi	a2,a4,-2
1c00aa80:	00c6fa63          	bgeu	a3,a2,1c00aa94 <pos_libc_prf+0x1bae>
1c00aa84:	0027c603          	lbu	a2,2(a5)
1c00aa88:	ffe74683          	lbu	a3,-2(a4)
1c00aa8c:	fec70f23          	sb	a2,-2(a4)
1c00aa90:	00d78123          	sb	a3,2(a5)
1c00aa94:	c802                	sw	zero,16(sp)
1c00aa96:	875fe06f          	j	1c00930a <pos_libc_prf+0x424>
1c00aa9a:	8f49                	or	a4,a4,a0
1c00aa9c:	fbf68693          	addi	a3,a3,-65
1c00aaa0:	00330993          	addi	s3,t1,3
1c00aaa4:	47e5                	li	a5,25
1c00aaa6:	20071a63          	bnez	a4,1c00acba <pos_libc_prf+0x1dd4>
1c00aaaa:	2ed7e563          	bltu	a5,a3,1c00ad94 <pos_libc_prf+0x1eae>
1c00aaae:	04900793          	li	a5,73
1c00aab2:	00f30023          	sb	a5,0(t1)
1c00aab6:	04e00793          	li	a5,78
1c00aaba:	00f300a3          	sb	a5,1(t1)
1c00aabe:	04600793          	li	a5,70
1c00aac2:	00f30123          	sb	a5,2(t1)
1c00aac6:	185c                	addi	a5,sp,52
1c00aac8:	000301a3          	sb	zero,3(t1)
1c00aacc:	40f989b3          	sub	s3,s3,a5
1c00aad0:	4401                	li	s0,0
1c00aad2:	d402                	sw	zero,40(sp)
1c00aad4:	cc02                	sw	zero,24(sp)
1c00aad6:	8daff06f          	j	1c009bb0 <pos_libc_prf+0xcca>
1c00aada:	1854                	addi	a3,sp,52
1c00aadc:	45e5                	li	a1,25
1c00aade:	0016c70b          	p.lbu	a4,1(a3!)
1c00aae2:	f9f70793          	addi	a5,a4,-97
1c00aae6:	0ff7f793          	andi	a5,a5,255
1c00aaea:	fe070613          	addi	a2,a4,-32
1c00aaee:	00f5ee63          	bltu	a1,a5,1c00ab0a <pos_libc_prf+0x1c24>
1c00aaf2:	fec68fa3          	sb	a2,-1(a3)
1c00aaf6:	0016c70b          	p.lbu	a4,1(a3!)
1c00aafa:	f9f70793          	addi	a5,a4,-97
1c00aafe:	0ff7f793          	andi	a5,a5,255
1c00ab02:	fe070613          	addi	a2,a4,-32
1c00ab06:	fef5f6e3          	bgeu	a1,a5,1c00aaf2 <pos_libc_prf+0x1c0c>
1c00ab0a:	fb71                	bnez	a4,1c00aade <pos_libc_prf+0x1bf8>
1c00ab0c:	9b7fe06f          	j	1c0094c2 <pos_libc_prf+0x5dc>
1c00ab10:	c0260613          	addi	a2,a2,-1022
1c00ab14:	57f9                	li	a5,-2
1c00ab16:	8eaa                	mv	t4,a0
1c00ab18:	c1f74f33          	p.bset	t5,a4,0,31
1c00ab1c:	4501                	li	a0,0
1c00ab1e:	98f64063          	blt	a2,a5,1c009c9e <pos_libc_prf+0xdb8>
1c00ab22:	9e8ff06f          	j	1c009d0a <pos_libc_prf+0xe24>
1c00ab26:	57b2                	lw	a5,44(sp)
1c00ab28:	1e078963          	beqz	a5,1c00ad1a <pos_libc_prf+0x1e34>
1c00ab2c:	03000793          	li	a5,48
1c00ab30:	02f10a23          	sb	a5,52(sp)
1c00ab34:	015867b3          	or	a5,a6,s5
1c00ab38:	03510513          	addi	a0,sp,53
1c00ab3c:	24078663          	beqz	a5,1c00ad88 <pos_libc_prf+0x1ea2>
1c00ab40:	89aa                	mv	s3,a0
1c00ab42:	459d                	li	a1,7
1c00ab44:	f8383733          	p.bclr	a4,a6,28,3
1c00ab48:	03070713          	addi	a4,a4,48
1c00ab4c:	01da9793          	slli	a5,s5,0x1d
1c00ab50:	86d6                	mv	a3,s5
1c00ab52:	00385613          	srli	a2,a6,0x3
1c00ab56:	834e                	mv	t1,s3
1c00ab58:	00e980ab          	p.sb	a4,1(s3!)
1c00ab5c:	003ada93          	srli	s5,s5,0x3
1c00ab60:	8742                	mv	a4,a6
1c00ab62:	00c7e833          	or	a6,a5,a2
1c00ab66:	fef9                	bnez	a3,1c00ab44 <pos_libc_prf+0x1c5e>
1c00ab68:	fce5eee3          	bltu	a1,a4,1c00ab44 <pos_libc_prf+0x1c5e>
1c00ab6c:	00098023          	sb	zero,0(s3)
1c00ab70:	0a657363          	bgeu	a0,t1,1c00ac16 <pos_libc_prf+0x1d30>
1c00ab74:	fff54713          	not	a4,a0
1c00ab78:	971a                	add	a4,a4,t1
1c00ab7a:	00175793          	srli	a5,a4,0x1
1c00ab7e:	00178e13          	addi	t3,a5,1
1c00ab82:	01c506b3          	add	a3,a0,t3
1c00ab86:	40f307b3          	sub	a5,t1,a5
1c00ab8a:	04f6b7b3          	p.sletu	a5,a3,a5
1c00ab8e:	04a9b6b3          	p.sletu	a3,s3,a0
1c00ab92:	8fd5                	or	a5,a5,a3
1c00ab94:	85aa                	mv	a1,a0
1c00ab96:	869a                	mv	a3,t1
1c00ab98:	c7c9                	beqz	a5,1c00ac22 <pos_libc_prf+0x1d3c>
1c00ab9a:	00673713          	sltiu	a4,a4,6
1c00ab9e:	e351                	bnez	a4,1c00ac22 <pos_libc_prf+0x1d3c>
1c00aba0:	c20e3733          	p.bclr	a4,t3,1,0
1c00aba4:	ffd30793          	addi	a5,t1,-3
1c00aba8:	972a                	add	a4,a4,a0
1c00abaa:	4190                	lw	a2,0(a1)
1c00abac:	4394                	lw	a3,0(a5)
1c00abae:	c2d67657          	pv.shufflei0.sci.b	a2,a2,27
1c00abb2:	fec7ae2b          	p.sw	a2,-4(a5!)
1c00abb6:	c2d6f6d7          	pv.shufflei0.sci.b	a3,a3,27
1c00abba:	00d5a22b          	p.sw	a3,4(a1!)
1c00abbe:	fee596e3          	bne	a1,a4,1c00abaa <pos_libc_prf+0x1cc4>
1c00abc2:	c20e36b3          	p.bclr	a3,t3,1,0
1c00abc6:	40d30733          	sub	a4,t1,a3
1c00abca:	00d507b3          	add	a5,a0,a3
1c00abce:	05c68463          	beq	a3,t3,1c00ac16 <pos_libc_prf+0x1d30>
1c00abd2:	0007c603          	lbu	a2,0(a5)
1c00abd6:	00074583          	lbu	a1,0(a4)
1c00abda:	86ba                	mv	a3,a4
1c00abdc:	fec68fab          	p.sb	a2,-1(a3!)
1c00abe0:	863e                	mv	a2,a5
1c00abe2:	00b600ab          	p.sb	a1,1(a2!)
1c00abe6:	02d67863          	bgeu	a2,a3,1c00ac16 <pos_libc_prf+0x1d30>
1c00abea:	0017c583          	lbu	a1,1(a5)
1c00abee:	fff74603          	lbu	a2,-1(a4)
1c00abf2:	00278693          	addi	a3,a5,2
1c00abf6:	feb70fa3          	sb	a1,-1(a4)
1c00abfa:	00c780a3          	sb	a2,1(a5)
1c00abfe:	ffe70613          	addi	a2,a4,-2
1c00ac02:	00c6fa63          	bgeu	a3,a2,1c00ac16 <pos_libc_prf+0x1d30>
1c00ac06:	0027c603          	lbu	a2,2(a5)
1c00ac0a:	ffe74683          	lbu	a3,-2(a4)
1c00ac0e:	fec70f23          	sb	a2,-2(a4)
1c00ac12:	00d78123          	sb	a3,2(a5)
1c00ac16:	185c                	addi	a5,sp,52
1c00ac18:	40f989b3          	sub	s3,s3,a5
1c00ac1c:	c802                	sw	zero,16(sp)
1c00ac1e:	eecfe06f          	j	1c00930a <pos_libc_prf+0x424>
1c00ac22:	00054703          	lbu	a4,0(a0)
1c00ac26:	0006c783          	lbu	a5,0(a3)
1c00ac2a:	fee68fab          	p.sb	a4,-1(a3!)
1c00ac2e:	00f500ab          	p.sb	a5,1(a0!)
1c00ac32:	fed568e3          	bltu	a0,a3,1c00ac22 <pos_libc_prf+0x1d3c>
1c00ac36:	b7c5                	j	1c00ac16 <pos_libc_prf+0x1d30>
1c00ac38:	03610793          	addi	a5,sp,54
1c00ac3c:	0007c603          	lbu	a2,0(a5)
1c00ac40:	00074683          	lbu	a3,0(a4)
1c00ac44:	fec70fab          	p.sb	a2,-1(a4!)
1c00ac48:	00d780ab          	p.sb	a3,1(a5!)
1c00ac4c:	fee7e8e3          	bltu	a5,a4,1c00ac3c <pos_libc_prf+0x1d56>
1c00ac50:	a38ff06f          	j	1c009e88 <pos_libc_prf+0xfa2>
1c00ac54:	01670023          	sb	s6,0(a4)
1c00ac58:	ae8fe06f          	j	1c008f40 <pos_libc_prf+0x5a>
1c00ac5c:	87c2                	mv	a5,a6
1c00ac5e:	0007c603          	lbu	a2,0(a5)
1c00ac62:	00074683          	lbu	a3,0(a4)
1c00ac66:	fec70fab          	p.sb	a2,-1(a4!)
1c00ac6a:	00d780ab          	p.sb	a3,1(a5!)
1c00ac6e:	fee7e8e3          	bltu	a5,a4,1c00ac5e <pos_libc_prf+0x1d78>
1c00ac72:	e80fe06f          	j	1c0092f2 <pos_libc_prf+0x40c>
1c00ac76:	4be2                	lw	s7,24(sp)
1c00ac78:	a039                	j	1c00ac86 <pos_libc_prf+0x1da0>
1c00ac7a:	9a02                	jalr	s4
1c00ac7c:	1bfd                	addi	s7,s7,-1
1c00ac7e:	01f53463          	p.bneimm	a0,-1,1c00ac86 <pos_libc_prf+0x1da0>
1c00ac82:	c44fe06f          	j	1c0090c6 <pos_libc_prf+0x1e0>
1c00ac86:	85a6                	mv	a1,s1
1c00ac88:	03000513          	li	a0,48
1c00ac8c:	ff7047e3          	bgtz	s7,1c00ac7a <pos_libc_prf+0x1d94>
1c00ac90:	47e2                	lw	a5,24(sp)
1c00ac92:	40fc0c33          	sub	s8,s8,a5
1c00ac96:	cfefe06f          	j	1c009194 <pos_libc_prf+0x2ae>
1c00ac9a:	00a407b3          	add	a5,s0,a0
1c00ac9e:	4581                	li	a1,0
1c00aca0:	a96ff06f          	j	1c009f36 <pos_libc_prf+0x1050>
1c00aca4:	1858                	addi	a4,sp,52
1c00aca6:	cc3a                	sw	a4,24(sp)
1c00aca8:	893a                	mv	s2,a4
1c00acaa:	d5cfe06f          	j	1c009206 <pos_libc_prf+0x320>
1c00acae:	896e                	mv	s2,s11
1c00acb0:	ca02                	sw	zero,20(sp)
1c00acb2:	c802                	sw	zero,16(sp)
1c00acb4:	4981                	li	s3,0
1c00acb6:	c7afe06f          	j	1c009130 <pos_libc_prf+0x24a>
1c00acba:	0ad7ec63          	bltu	a5,a3,1c00ad72 <pos_libc_prf+0x1e8c>
1c00acbe:	04e00793          	li	a5,78
1c00acc2:	04100713          	li	a4,65
1c00acc6:	00f30023          	sb	a5,0(t1)
1c00acca:	00e300a3          	sb	a4,1(t1)
1c00acce:	00f30123          	sb	a5,2(t1)
1c00acd2:	bbd5                	j	1c00aac6 <pos_libc_prf+0x1be0>
1c00acd4:	06600693          	li	a3,102
1c00acd8:	4781                	li	a5,0
1c00acda:	080002b7          	lui	t0,0x8000
1c00acde:	9effe06f          	j	1c0096cc <pos_libc_prf+0x7e6>
1c00ace2:	03000613          	li	a2,48
1c00ace6:	00c30023          	sb	a2,0(t1)
1c00acea:	5632                	lw	a2,44(sp)
1c00acec:	e239                	bnez	a2,1c00ad32 <pos_libc_prf+0x1e4c>
1c00acee:	c061                	beqz	s0,1c00adae <pos_libc_prf+0x1ec8>
1c00acf0:	02e00713          	li	a4,46
1c00acf4:	00e300a3          	sb	a4,1(t1)
1c00acf8:	0309                	addi	t1,t1,2
1c00acfa:	14050c63          	beqz	a0,1c00ae52 <pos_libc_prf+0x1f6c>
1c00acfe:	40a00733          	neg	a4,a0
1c00ad02:	04874eb3          	p.min	t4,a4,s0
1c00ad06:	d476                	sw	t4,40(sp)
1c00ad08:	41d40e33          	sub	t3,s0,t4
1c00ad0c:	4641                	li	a2,16
1c00ad0e:	cc02                	sw	zero,24(sp)
1c00ad10:	fbc04163          	bgtz	t3,1c00a4b2 <pos_libc_prf+0x15cc>
1c00ad14:	8472                	mv	s0,t3
1c00ad16:	871a                	mv	a4,t1
1c00ad18:	b925                	j	1c00a950 <pos_libc_prf+0x1a6a>
1c00ad1a:	1848                	addi	a0,sp,52
1c00ad1c:	b515                	j	1c00ab40 <pos_libc_prf+0x1c5a>
1c00ad1e:	833a                	mv	t1,a4
1c00ad20:	02e00713          	li	a4,46
1c00ad24:	4ee2                	lw	t4,24(sp)
1c00ad26:	00e300ab          	p.sb	a4,1(t1!)
1c00ad2a:	8e22                	mv	t3,s0
1c00ad2c:	4501                	li	a0,0
1c00ad2e:	d402                	sw	zero,40(sp)
1c00ad30:	b7c5                	j	1c00ad10 <pos_libc_prf+0x1e2a>
1c00ad32:	02e00713          	li	a4,46
1c00ad36:	00e300a3          	sb	a4,1(t1)
1c00ad3a:	0309                	addi	t1,t1,2
1c00ad3c:	14050363          	beqz	a0,1c00ae82 <pos_libc_prf+0x1f9c>
1c00ad40:	fc5d                	bnez	s0,1c00acfe <pos_libc_prf+0x1e18>
1c00ad42:	cc02                	sw	zero,24(sp)
1c00ad44:	4e81                	li	t4,0
1c00ad46:	d402                	sw	zero,40(sp)
1c00ad48:	871a                	mv	a4,t1
1c00ad4a:	b119                	j	1c00a950 <pos_libc_prf+0x1a6a>
1c00ad4c:	87a2                	mv	a5,s0
1c00ad4e:	4581                	li	a1,0
1c00ad50:	8436                	mv	s0,a3
1c00ad52:	9e4ff06f          	j	1c009f36 <pos_libc_prf+0x1050>
1c00ad56:	185c                	addi	a5,sp,52
1c00ad58:	0007c603          	lbu	a2,0(a5)
1c00ad5c:	0006c703          	lbu	a4,0(a3)
1c00ad60:	fec68fab          	p.sb	a2,-1(a3!)
1c00ad64:	00e780ab          	p.sb	a4,1(a5!)
1c00ad68:	fed7e8e3          	bltu	a5,a3,1c00ad58 <pos_libc_prf+0x1e72>
1c00ad6c:	c802                	sw	zero,16(sp)
1c00ad6e:	d9cfe06f          	j	1c00930a <pos_libc_prf+0x424>
1c00ad72:	06e00793          	li	a5,110
1c00ad76:	06100713          	li	a4,97
1c00ad7a:	00f30023          	sb	a5,0(t1)
1c00ad7e:	00e300a3          	sb	a4,1(t1)
1c00ad82:	00f30123          	sb	a5,2(t1)
1c00ad86:	b381                	j	1c00aac6 <pos_libc_prf+0x1be0>
1c00ad88:	02010aa3          	sb	zero,53(sp)
1c00ad8c:	c802                	sw	zero,16(sp)
1c00ad8e:	4985                	li	s3,1
1c00ad90:	d7afe06f          	j	1c00930a <pos_libc_prf+0x424>
1c00ad94:	06900793          	li	a5,105
1c00ad98:	00f30023          	sb	a5,0(t1)
1c00ad9c:	06e00793          	li	a5,110
1c00ada0:	00f300a3          	sb	a5,1(t1)
1c00ada4:	06600793          	li	a5,102
1c00ada8:	00f30123          	sb	a5,2(t1)
1c00adac:	bb29                	j	1c00aac6 <pos_libc_prf+0x1be0>
1c00adae:	cc02                	sw	zero,24(sp)
1c00adb0:	c509                	beqz	a0,1c00adba <pos_libc_prf+0x1ed4>
1c00adb2:	4e81                	li	t4,0
1c00adb4:	d402                	sw	zero,40(sp)
1c00adb6:	be69                	j	1c00a950 <pos_libc_prf+0x1a6a>
1c00adb8:	4562                	lw	a0,24(sp)
1c00adba:	4ee2                	lw	t4,24(sp)
1c00adbc:	d402                	sw	zero,40(sp)
1c00adbe:	cc2a                	sw	a0,24(sp)
1c00adc0:	4401                	li	s0,0
1c00adc2:	4501                	li	a0,0
1c00adc4:	b671                	j	1c00a950 <pos_libc_prf+0x1a6a>
1c00adc6:	00860713          	addi	a4,a2,8
1c00adca:	8472                	mv	s0,t3
1c00adcc:	da7fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00add0:	00760713          	addi	a4,a2,7
1c00add4:	843e                	mv	s0,a5
1c00add6:	d9dfe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00adda:	00660713          	addi	a4,a2,6
1c00adde:	8472                	mv	s0,t3
1c00ade0:	d93fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ade4:	00560713          	addi	a4,a2,5
1c00ade8:	843e                	mv	s0,a5
1c00adea:	d89fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00adee:	00460713          	addi	a4,a2,4
1c00adf2:	8476                	mv	s0,t4
1c00adf4:	d7ffe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00adf8:	00360713          	addi	a4,a2,3
1c00adfc:	847a                	mv	s0,t5
1c00adfe:	d75fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae02:	00260713          	addi	a4,a2,2
1c00ae06:	4401                	li	s0,0
1c00ae08:	d6bfe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae0c:	00160713          	addi	a4,a2,1
1c00ae10:	4401                	li	s0,0
1c00ae12:	d61fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae16:	00c60713          	addi	a4,a2,12
1c00ae1a:	8472                	mv	s0,t3
1c00ae1c:	d57fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae20:	00b60713          	addi	a4,a2,11
1c00ae24:	8472                	mv	s0,t3
1c00ae26:	d4dfe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae2a:	00a60713          	addi	a4,a2,10
1c00ae2e:	8472                	mv	s0,t3
1c00ae30:	d43fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae34:	00960713          	addi	a4,a2,9
1c00ae38:	843e                	mv	s0,a5
1c00ae3a:	d39fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae3e:	00e60713          	addi	a4,a2,14
1c00ae42:	8476                	mv	s0,t4
1c00ae44:	d2ffe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae48:	00d60713          	addi	a4,a2,13
1c00ae4c:	843e                	mv	s0,a5
1c00ae4e:	d25fe06f          	j	1c009b72 <pos_libc_prf+0xc8c>
1c00ae52:	cc02                	sw	zero,24(sp)
1c00ae54:	4641                	li	a2,16
1c00ae56:	e54ff06f          	j	1c00a4aa <pos_libc_prf+0x15c4>
1c00ae5a:	00f30713          	addi	a4,t1,15
1c00ae5e:	4605                	li	a2,1
1c00ae60:	e36ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00ae64:	00b30713          	addi	a4,t1,11
1c00ae68:	4615                	li	a2,5
1c00ae6a:	e2cff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00ae6e:	00a30713          	addi	a4,t1,10
1c00ae72:	4619                	li	a2,6
1c00ae74:	e22ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00ae78:	00930713          	addi	a4,t1,9
1c00ae7c:	461d                	li	a2,7
1c00ae7e:	e18ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00ae82:	cc02                	sw	zero,24(sp)
1c00ae84:	d402                	sw	zero,40(sp)
1c00ae86:	4e81                	li	t4,0
1c00ae88:	8e22                	mv	t3,s0
1c00ae8a:	4641                	li	a2,16
1c00ae8c:	e2804563          	bgtz	s0,1c00a4b6 <pos_libc_prf+0x15d0>
1c00ae90:	b551                	j	1c00ad14 <pos_libc_prf+0x1e2e>
1c00ae92:	00e30713          	addi	a4,t1,14
1c00ae96:	4609                	li	a2,2
1c00ae98:	dfeff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00ae9c:	00d30713          	addi	a4,t1,13
1c00aea0:	460d                	li	a2,3
1c00aea2:	df4ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aea6:	00830713          	addi	a4,t1,8
1c00aeaa:	4621                	li	a2,8
1c00aeac:	deaff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aeb0:	00730713          	addi	a4,t1,7
1c00aeb4:	4625                	li	a2,9
1c00aeb6:	de0ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aeba:	00630713          	addi	a4,t1,6
1c00aebe:	4629                	li	a2,10
1c00aec0:	dd6ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aec4:	00530713          	addi	a4,t1,5
1c00aec8:	462d                	li	a2,11
1c00aeca:	dccff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aece:	00430713          	addi	a4,t1,4
1c00aed2:	4631                	li	a2,12
1c00aed4:	dc2ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aed8:	00330713          	addi	a4,t1,3
1c00aedc:	4635                	li	a2,13
1c00aede:	db8ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aee2:	00230713          	addi	a4,t1,2
1c00aee6:	4639                	li	a2,14
1c00aee8:	cc02                	sw	zero,24(sp)
1c00aeea:	dacff06f          	j	1c00a496 <pos_libc_prf+0x15b0>
1c00aeee:	463d                	li	a2,15
1c00aef0:	cc02                	sw	zero,24(sp)
1c00aef2:	da4ff06f          	j	1c00a496 <pos_libc_prf+0x15b0>

1c00aef6 <pos_init_start>:
1c00aef6:	1141                	addi	sp,sp,-16
1c00aef8:	c422                	sw	s0,8(sp)
1c00aefa:	1c001437          	lui	s0,0x1c001
1c00aefe:	c606                	sw	ra,12(sp)
1c00af00:	84840413          	addi	s0,s0,-1976 # 1c000848 <ctor_list>
1c00af04:	22f5                	jal	1c00b0f0 <pos_irq_init>
1c00af06:	d2bfd0ef          	jal	ra,1c008c30 <pos_soc_init>
1c00af0a:	2c21                	jal	1c00b122 <pos_soc_event_init>
1c00af0c:	2a15                	jal	1c00b040 <pos_allocs_init>
1c00af0e:	284d                	jal	1c00afc0 <pos_sched_init>
1c00af10:	405c                	lw	a5,4(s0)
1c00af12:	c791                	beqz	a5,1c00af1e <pos_init_start+0x28>
1c00af14:	0421                	addi	s0,s0,8
1c00af16:	9782                	jalr	a5
1c00af18:	0044278b          	p.lw	a5,4(s0!)
1c00af1c:	ffed                	bnez	a5,1c00af16 <pos_init_start+0x20>
1c00af1e:	f2ffd0ef          	jal	ra,1c008e4c <pos_io_start>
1c00af22:	300467f3          	csrrsi	a5,mstatus,8
1c00af26:	40b2                	lw	ra,12(sp)
1c00af28:	4422                	lw	s0,8(sp)
1c00af2a:	0141                	addi	sp,sp,16
1c00af2c:	8082                	ret

1c00af2e <pos_init_stop>:
1c00af2e:	1141                	addi	sp,sp,-16
1c00af30:	c422                	sw	s0,8(sp)
1c00af32:	1c001437          	lui	s0,0x1c001
1c00af36:	c606                	sw	ra,12(sp)
1c00af38:	86040413          	addi	s0,s0,-1952 # 1c000860 <dtor_list>
1c00af3c:	f57fd0ef          	jal	ra,1c008e92 <pos_io_stop>
1c00af40:	405c                	lw	a5,4(s0)
1c00af42:	c791                	beqz	a5,1c00af4e <pos_init_stop+0x20>
1c00af44:	0421                	addi	s0,s0,8
1c00af46:	9782                	jalr	a5
1c00af48:	0044278b          	p.lw	a5,4(s0!)
1c00af4c:	ffed                	bnez	a5,1c00af46 <pos_init_stop+0x18>
1c00af4e:	40b2                	lw	ra,12(sp)
1c00af50:	4422                	lw	s0,8(sp)
1c00af52:	0141                	addi	sp,sp,16
1c00af54:	8082                	ret

1c00af56 <pos_cbsys_add>:
1c00af56:	00259793          	slli	a5,a1,0x2
1c00af5a:	1c0015b7          	lui	a1,0x1c001
1c00af5e:	3d058593          	addi	a1,a1,976 # 1c0013d0 <pos_cbsys_first>
1c00af62:	95be                	add	a1,a1,a5
1c00af64:	419c                	lw	a5,0(a1)
1c00af66:	c110                	sw	a2,0(a0)
1c00af68:	c154                	sw	a3,4(a0)
1c00af6a:	c51c                	sw	a5,8(a0)
1c00af6c:	c188                	sw	a0,0(a1)
1c00af6e:	8082                	ret

1c00af70 <pi_open_from_conf>:
1c00af70:	c14c                	sw	a1,4(a0)
1c00af72:	8082                	ret

1c00af74 <pos_task_handle_blocking>:
1c00af74:	4785                	li	a5,1
1c00af76:	00f50a23          	sb	a5,20(a0)
1c00af7a:	8082                	ret

1c00af7c <pos_task_handle>:
1c00af7c:	1141                	addi	sp,sp,-16
1c00af7e:	c422                	sw	s0,8(sp)
1c00af80:	01c00413          	li	s0,28
1c00af84:	401c                	lw	a5,0(s0)
1c00af86:	c606                	sw	ra,12(sp)
1c00af88:	c385                	beqz	a5,1c00afa8 <pos_task_handle+0x2c>
1c00af8a:	4394                	lw	a3,0(a5)
1c00af8c:	43d8                	lw	a4,4(a5)
1c00af8e:	4788                	lw	a0,8(a5)
1c00af90:	c014                	sw	a3,0(s0)
1c00af92:	300467f3          	csrrsi	a5,mstatus,8
1c00af96:	9702                	jalr	a4
1c00af98:	300477f3          	csrrci	a5,mstatus,8
1c00af9c:	401c                	lw	a5,0(s0)
1c00af9e:	f7f5                	bnez	a5,1c00af8a <pos_task_handle+0xe>
1c00afa0:	40b2                	lw	ra,12(sp)
1c00afa2:	4422                	lw	s0,8(sp)
1c00afa4:	0141                	addi	sp,sp,16
1c00afa6:	8082                	ret
1c00afa8:	10500073          	wfi
1c00afac:	300467f3          	csrrsi	a5,mstatus,8
1c00afb0:	300477f3          	csrrci	a5,mstatus,8
1c00afb4:	401c                	lw	a5,0(s0)
1c00afb6:	fbf1                	bnez	a5,1c00af8a <pos_task_handle+0xe>
1c00afb8:	40b2                	lw	ra,12(sp)
1c00afba:	4422                	lw	s0,8(sp)
1c00afbc:	0141                	addi	sp,sp,16
1c00afbe:	8082                	ret

1c00afc0 <pos_sched_init>:
1c00afc0:	00002e23          	sw	zero,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00afc4:	8082                	ret

1c00afc6 <pos_alloc_init>:
1c00afc6:	00758793          	addi	a5,a1,7
1c00afca:	c407b7b3          	p.bclr	a5,a5,2,0
1c00afce:	40b785b3          	sub	a1,a5,a1
1c00afd2:	c11c                	sw	a5,0(a0)
1c00afd4:	40b605b3          	sub	a1,a2,a1
1c00afd8:	00b05763          	blez	a1,1c00afe6 <pos_alloc_init+0x20>
1c00afdc:	c405b5b3          	p.bclr	a1,a1,2,0
1c00afe0:	c38c                	sw	a1,0(a5)
1c00afe2:	0007a223          	sw	zero,4(a5)
1c00afe6:	8082                	ret

1c00afe8 <pos_alloc>:
1c00afe8:	411c                	lw	a5,0(a0)
1c00afea:	00758713          	addi	a4,a1,7
1c00afee:	862a                	mv	a2,a0
1c00aff0:	c40735b3          	p.bclr	a1,a4,2,0
1c00aff4:	4681                	li	a3,0
1c00aff6:	e789                	bnez	a5,1c00b000 <pos_alloc+0x18>
1c00aff8:	a091                	j	1c00b03c <pos_alloc+0x54>
1c00affa:	86be                	mv	a3,a5
1c00affc:	c505                	beqz	a0,1c00b024 <pos_alloc+0x3c>
1c00affe:	87aa                	mv	a5,a0
1c00b000:	4398                	lw	a4,0(a5)
1c00b002:	43c8                	lw	a0,4(a5)
1c00b004:	feb74be3          	blt	a4,a1,1c00affa <pos_alloc+0x12>
1c00b008:	00b70f63          	beq	a4,a1,1c00b026 <pos_alloc+0x3e>
1c00b00c:	00b78833          	add	a6,a5,a1
1c00b010:	8f0d                	sub	a4,a4,a1
1c00b012:	00e82023          	sw	a4,0(a6)
1c00b016:	00a82223          	sw	a0,4(a6)
1c00b01a:	ca91                	beqz	a3,1c00b02e <pos_alloc+0x46>
1c00b01c:	0106a223          	sw	a6,4(a3)
1c00b020:	853e                	mv	a0,a5
1c00b022:	8082                	ret
1c00b024:	8082                	ret
1c00b026:	ca81                	beqz	a3,1c00b036 <pos_alloc+0x4e>
1c00b028:	c2c8                	sw	a0,4(a3)
1c00b02a:	853e                	mv	a0,a5
1c00b02c:	8082                	ret
1c00b02e:	01062023          	sw	a6,0(a2)
1c00b032:	853e                	mv	a0,a5
1c00b034:	8082                	ret
1c00b036:	c208                	sw	a0,0(a2)
1c00b038:	853e                	mv	a0,a5
1c00b03a:	8082                	ret
1c00b03c:	4501                	li	a0,0
1c00b03e:	8082                	ret

1c00b040 <pos_allocs_init>:
1c00b040:	1141                	addi	sp,sp,-16
1c00b042:	1c0015b7          	lui	a1,0x1c001
1c00b046:	44858613          	addi	a2,a1,1096 # 1c001448 <__l2_priv0_end>
1c00b04a:	1c0087b7          	lui	a5,0x1c008
1c00b04e:	c606                	sw	ra,12(sp)
1c00b050:	c422                	sw	s0,8(sp)
1c00b052:	44858593          	addi	a1,a1,1096
1c00b056:	40c78633          	sub	a2,a5,a2
1c00b05a:	04f5d163          	bge	a1,a5,1c00b09c <pos_allocs_init+0x5c>
1c00b05e:	1c001437          	lui	s0,0x1c001
1c00b062:	42040513          	addi	a0,s0,1056 # 1c001420 <pos_alloc_l2>
1c00b066:	3785                	jal	1c00afc6 <pos_alloc_init>
1c00b068:	1c0105b7          	lui	a1,0x1c010
1c00b06c:	40858793          	addi	a5,a1,1032 # 1c010408 <__l2_shared_end>
1c00b070:	1c200637          	lui	a2,0x1c200
1c00b074:	1c001537          	lui	a0,0x1c001
1c00b078:	8e1d                	sub	a2,a2,a5
1c00b07a:	40858593          	addi	a1,a1,1032
1c00b07e:	42450513          	addi	a0,a0,1060 # 1c001424 <pos_alloc_l2+0x4>
1c00b082:	3791                	jal	1c00afc6 <pos_alloc_init>
1c00b084:	42040513          	addi	a0,s0,1056
1c00b088:	4591                	li	a1,4
1c00b08a:	3fb9                	jal	1c00afe8 <pos_alloc>
1c00b08c:	40b2                	lw	ra,12(sp)
1c00b08e:	4422                	lw	s0,8(sp)
1c00b090:	1c0017b7          	lui	a5,0x1c001
1c00b094:	40a7ae23          	sw	a0,1052(a5) # 1c00141c <pos_alloc_l1>
1c00b098:	0141                	addi	sp,sp,16
1c00b09a:	8082                	ret
1c00b09c:	4581                	li	a1,0
1c00b09e:	4601                	li	a2,0
1c00b0a0:	bf7d                	j	1c00b05e <pos_allocs_init+0x1e>

1c00b0a2 <pos_irq_set_handler>:
1c00b0a2:	f14027f3          	csrr	a5,mhartid
1c00b0a6:	477d                	li	a4,31
1c00b0a8:	ca5797b3          	p.extractu	a5,a5,5,5
1c00b0ac:	02e78d63          	beq	a5,a4,1c00b0e6 <pos_irq_set_handler+0x44>
1c00b0b0:	002007b7          	lui	a5,0x200
1c00b0b4:	43b8                	lw	a4,64(a5)
1c00b0b6:	050a                	slli	a0,a0,0x2
1c00b0b8:	8d89                	sub	a1,a1,a0
1c00b0ba:	8d99                	sub	a1,a1,a4
1c00b0bc:	c14586b3          	p.extract	a3,a1,0,20
1c00b0c0:	06f00793          	li	a5,111
1c00b0c4:	c1f6a7b3          	p.insert	a5,a3,0,31
1c00b0c8:	d21586b3          	p.extract	a3,a1,9,1
1c00b0cc:	d356a7b3          	p.insert	a5,a3,9,21
1c00b0d0:	c0b586b3          	p.extract	a3,a1,0,11
1c00b0d4:	c146a7b3          	p.insert	a5,a3,0,20
1c00b0d8:	cec585b3          	p.extract	a1,a1,7,12
1c00b0dc:	953a                	add	a0,a0,a4
1c00b0de:	cec5a7b3          	p.insert	a5,a1,7,12
1c00b0e2:	c11c                	sw	a5,0(a0)
1c00b0e4:	8082                	ret
1c00b0e6:	30502773          	csrr	a4,mtvec
1c00b0ea:	c0073733          	p.bclr	a4,a4,0,0
1c00b0ee:	b7e1                	j	1c00b0b6 <pos_irq_set_handler+0x14>

1c00b0f0 <pos_irq_init>:
1c00b0f0:	f1402773          	csrr	a4,mhartid
1c00b0f4:	1a10a7b7          	lui	a5,0x1a10a
1c00b0f8:	56fd                	li	a3,-1
1c00b0fa:	80d7a423          	sw	a3,-2040(a5) # 1a109808 <__l1_end+0xa1097e8>
1c00b0fe:	ca571733          	p.extractu	a4,a4,5,5
1c00b102:	1c0087b7          	lui	a5,0x1c008
1c00b106:	46fd                	li	a3,31
1c00b108:	00078793          	mv	a5,a5
1c00b10c:	00d70663          	beq	a4,a3,1c00b118 <pos_irq_init+0x28>
1c00b110:	00200737          	lui	a4,0x200
1c00b114:	c33c                	sw	a5,64(a4)
1c00b116:	8082                	ret
1c00b118:	c007c7b3          	p.bset	a5,a5,0,0
1c00b11c:	30579073          	csrw	mtvec,a5
1c00b120:	8082                	ret

1c00b122 <pos_soc_event_init>:
1c00b122:	1141                	addi	sp,sp,-16
1c00b124:	c606                	sw	ra,12(sp)
1c00b126:	1a106737          	lui	a4,0x1a106
1c00b12a:	57fd                	li	a5,-1
1c00b12c:	c35c                	sw	a5,4(a4)
1c00b12e:	c71c                	sw	a5,8(a4)
1c00b130:	c75c                	sw	a5,12(a4)
1c00b132:	cb1c                	sw	a5,16(a4)
1c00b134:	cb5c                	sw	a5,20(a4)
1c00b136:	cf1c                	sw	a5,24(a4)
1c00b138:	cf5c                	sw	a5,28(a4)
1c00b13a:	1c00b5b7          	lui	a1,0x1c00b
1c00b13e:	02070693          	addi	a3,a4,32 # 1a106020 <__l1_end+0xa106000>
1c00b142:	c29c                	sw	a5,0(a3)
1c00b144:	76458593          	addi	a1,a1,1892 # 1c00b764 <pos_soc_event_handler_asm>
1c00b148:	4569                	li	a0,26
1c00b14a:	3fa1                	jal	1c00b0a2 <pos_irq_set_handler>
1c00b14c:	40b2                	lw	ra,12(sp)
1c00b14e:	1a10a7b7          	lui	a5,0x1a10a
1c00b152:	04000737          	lui	a4,0x4000
1c00b156:	80e7a223          	sw	a4,-2044(a5) # 1a109804 <__l1_end+0xa1097e4>
1c00b15a:	0141                	addi	sp,sp,16
1c00b15c:	8082                	ret

1c00b15e <pos_time_poweroff>:
1c00b15e:	1a10b7b7          	lui	a5,0x1a10b
1c00b162:	0791                	addi	a5,a5,4
1c00b164:	0087a783          	lw	a5,8(a5) # 1a10b008 <__l1_end+0xa10afe8>
1c00b168:	1c001737          	lui	a4,0x1c001
1c00b16c:	3ef72423          	sw	a5,1000(a4) # 1c0013e8 <pos_time_timer_count>
1c00b170:	4501                	li	a0,0
1c00b172:	8082                	ret

1c00b174 <pos_time_poweron>:
1c00b174:	1c0017b7          	lui	a5,0x1c001
1c00b178:	3e87a703          	lw	a4,1000(a5) # 1c0013e8 <pos_time_timer_count>
1c00b17c:	1a10b7b7          	lui	a5,0x1a10b
1c00b180:	0791                	addi	a5,a5,4
1c00b182:	00e7a423          	sw	a4,8(a5) # 1a10b008 <__l1_end+0xa10afe8>
1c00b186:	4501                	li	a0,0
1c00b188:	8082                	ret

1c00b18a <pos_time_timer_handler>:
1c00b18a:	1c0018b7          	lui	a7,0x1c001
1c00b18e:	3e888893          	addi	a7,a7,1000 # 1c0013e8 <pos_time_timer_count>
1c00b192:	0048a783          	lw	a5,4(a7)
1c00b196:	1a10b637          	lui	a2,0x1a10b
1c00b19a:	0611                	addi	a2,a2,4
1c00b19c:	00862603          	lw	a2,8(a2) # 1a10b008 <__l1_end+0xa10afe8>
1c00b1a0:	cfb5                	beqz	a5,1c00b21c <pos_time_timer_handler+0x92>
1c00b1a2:	80000537          	lui	a0,0x80000
1c00b1a6:	01c02583          	lw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b1aa:	02002683          	lw	a3,32(zero) # 20 <pos_sched_last>
1c00b1ae:	4801                	li	a6,0
1c00b1b0:	4301                	li	t1,0
1c00b1b2:	ffe54513          	xori	a0,a0,-2
1c00b1b6:	a031                	j	1c00b1c2 <pos_time_timer_handler+0x38>
1c00b1b8:	c29c                	sw	a5,0(a3)
1c00b1ba:	cb39                	beqz	a4,1c00b210 <pos_time_timer_handler+0x86>
1c00b1bc:	86be                	mv	a3,a5
1c00b1be:	4805                	li	a6,1
1c00b1c0:	87ba                	mv	a5,a4
1c00b1c2:	5fd8                	lw	a4,60(a5)
1c00b1c4:	40e60733          	sub	a4,a2,a4
1c00b1c8:	00e56a63          	bltu	a0,a4,1c00b1dc <pos_time_timer_handler+0x52>
1c00b1cc:	4398                	lw	a4,0(a5)
1c00b1ce:	0007a023          	sw	zero,0(a5)
1c00b1d2:	f1fd                	bnez	a1,1c00b1b8 <pos_time_timer_handler+0x2e>
1c00b1d4:	85be                	mv	a1,a5
1c00b1d6:	cf1d                	beqz	a4,1c00b214 <pos_time_timer_handler+0x8a>
1c00b1d8:	4305                	li	t1,1
1c00b1da:	b7cd                	j	1c00b1bc <pos_time_timer_handler+0x32>
1c00b1dc:	00030463          	beqz	t1,1c00b1e4 <pos_time_timer_handler+0x5a>
1c00b1e0:	00b02e23          	sw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b1e4:	00080463          	beqz	a6,1c00b1ec <pos_time_timer_handler+0x62>
1c00b1e8:	02d02023          	sw	a3,32(zero) # 20 <pos_sched_last>
1c00b1ec:	1a10b737          	lui	a4,0x1a10b
1c00b1f0:	0711                	addi	a4,a4,4
1c00b1f2:	00f8a223          	sw	a5,4(a7)
1c00b1f6:	00872683          	lw	a3,8(a4) # 1a10b008 <__l1_end+0xa10afe8>
1c00b1fa:	5fdc                	lw	a5,60(a5)
1c00b1fc:	40c78633          	sub	a2,a5,a2
1c00b200:	9636                	add	a2,a2,a3
1c00b202:	00c72823          	sw	a2,16(a4)
1c00b206:	08500793          	li	a5,133
1c00b20a:	00f72023          	sw	a5,0(a4)
1c00b20e:	8082                	ret
1c00b210:	00030463          	beqz	t1,1c00b218 <pos_time_timer_handler+0x8e>
1c00b214:	00b02e23          	sw	a1,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b218:	02f02023          	sw	a5,32(zero) # 20 <pos_sched_last>
1c00b21c:	1a10b7b7          	lui	a5,0x1a10b
1c00b220:	08100713          	li	a4,129
1c00b224:	0791                	addi	a5,a5,4
1c00b226:	0008a223          	sw	zero,4(a7)
1c00b22a:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c00b22e:	f14027f3          	csrr	a5,mhartid
1c00b232:	477d                	li	a4,31
1c00b234:	ca5797b3          	p.extractu	a5,a5,5,5
1c00b238:	00e78a63          	beq	a5,a4,1c00b24c <pos_time_timer_handler+0xc2>
1c00b23c:	6785                	lui	a5,0x1
1c00b23e:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00b242:	00204737          	lui	a4,0x204
1c00b246:	02f72423          	sw	a5,40(a4) # 204028 <__l1_heap_size+0x1c4048>
1c00b24a:	8082                	ret
1c00b24c:	6785                	lui	a5,0x1
1c00b24e:	1a10a737          	lui	a4,0x1a10a
1c00b252:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00b256:	80f72a23          	sw	a5,-2028(a4) # 1a109814 <__l1_end+0xa1097f4>
1c00b25a:	8082                	ret

1c00b25c <pos_time_init>:
1c00b25c:	1141                	addi	sp,sp,-16
1c00b25e:	c422                	sw	s0,8(sp)
1c00b260:	1a10b7b7          	lui	a5,0x1a10b
1c00b264:	1c001437          	lui	s0,0x1c001
1c00b268:	3e840413          	addi	s0,s0,1000 # 1c0013e8 <pos_time_timer_count>
1c00b26c:	08300713          	li	a4,131
1c00b270:	0791                	addi	a5,a5,4
1c00b272:	c606                	sw	ra,12(sp)
1c00b274:	00042223          	sw	zero,4(s0)
1c00b278:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10afe0>
1c00b27c:	1c00b5b7          	lui	a1,0x1c00b
1c00b280:	74058593          	addi	a1,a1,1856 # 1c00b740 <pos_time_timer_handler_asm>
1c00b284:	452d                	li	a0,11
1c00b286:	3d31                	jal	1c00b0a2 <pos_irq_set_handler>
1c00b288:	6785                	lui	a5,0x1
1c00b28a:	1a10a737          	lui	a4,0x1a10a
1c00b28e:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3bc>
1c00b292:	1c00b637          	lui	a2,0x1c00b
1c00b296:	00840513          	addi	a0,s0,8
1c00b29a:	4681                	li	a3,0
1c00b29c:	15e60613          	addi	a2,a2,350 # 1c00b15e <pos_time_poweroff>
1c00b2a0:	4589                	li	a1,2
1c00b2a2:	80f72223          	sw	a5,-2044(a4) # 1a109804 <__l1_end+0xa1097e4>
1c00b2a6:	3945                	jal	1c00af56 <pos_cbsys_add>
1c00b2a8:	01840513          	addi	a0,s0,24
1c00b2ac:	4422                	lw	s0,8(sp)
1c00b2ae:	40b2                	lw	ra,12(sp)
1c00b2b0:	1c00b637          	lui	a2,0x1c00b
1c00b2b4:	4681                	li	a3,0
1c00b2b6:	17460613          	addi	a2,a2,372 # 1c00b174 <pos_time_poweron>
1c00b2ba:	458d                	li	a1,3
1c00b2bc:	0141                	addi	sp,sp,16
1c00b2be:	b961                	j	1c00af56 <pos_cbsys_add>

1c00b2c0 <pi_uart_conf_init>:
1c00b2c0:	6771                	lui	a4,0x1c
1c00b2c2:	4785                	li	a5,1
1c00b2c4:	20070713          	addi	a4,a4,512 # 1c200 <pos_soc_event_callback+0x1bdbc>
1c00b2c8:	c118                	sw	a4,0(a0)
1c00b2ca:	00f51223          	sh	a5,4(a0) # 80000004 <__l2_shared_end+0x63fefbfc>
1c00b2ce:	00f503a3          	sb	a5,7(a0)
1c00b2d2:	00f51423          	sh	a5,8(a0)
1c00b2d6:	8082                	ret

1c00b2d8 <pi_uart_open>:
1c00b2d8:	7179                	addi	sp,sp,-48
1c00b2da:	d606                	sw	ra,44(sp)
1c00b2dc:	d422                	sw	s0,40(sp)
1c00b2de:	d226                	sw	s1,36(sp)
1c00b2e0:	d04a                	sw	s2,32(sp)
1c00b2e2:	ce4e                	sw	s3,28(sp)
1c00b2e4:	cc52                	sw	s4,24(sp)
1c00b2e6:	ca56                	sw	s5,20(sp)
1c00b2e8:	c85a                	sw	s6,16(sp)
1c00b2ea:	c65e                	sw	s7,12(sp)
1c00b2ec:	c462                	sw	s8,8(sp)
1c00b2ee:	c266                	sw	s9,4(sp)
1c00b2f0:	300479f3          	csrrci	s3,mstatus,8
1c00b2f4:	4158                	lw	a4,4(a0)
1c00b2f6:	1c0107b7          	lui	a5,0x1c010
1c00b2fa:	3b478913          	addi	s2,a5,948 # 1c0103b4 <pos_uart>
1c00b2fe:	00974483          	lbu	s1,9(a4)
1c00b302:	00072c03          	lw	s8,0(a4)
1c00b306:	00349a13          	slli	s4,s1,0x3
1c00b30a:	409a0bb3          	sub	s7,s4,s1
1c00b30e:	0b8e                	slli	s7,s7,0x3
1c00b310:	01790ab3          	add	s5,s2,s7
1c00b314:	000aa703          	lw	a4,0(s5)
1c00b318:	00249413          	slli	s0,s1,0x2
1c00b31c:	01552423          	sw	s5,8(a0)
1c00b320:	00140b13          	addi	s6,s0,1
1c00b324:	cb55                	beqz	a4,1c00b3d8 <pi_uart_open+0x100>
1c00b326:	409a07b3          	sub	a5,s4,s1
1c00b32a:	078e                	slli	a5,a5,0x3
1c00b32c:	97ca                	add	a5,a5,s2
1c00b32e:	0705                	addi	a4,a4,1
1c00b330:	c398                	sw	a4,0(a5)
1c00b332:	c3c0                	sw	s0,4(a5)
1c00b334:	0187a423          	sw	s8,8(a5)
1c00b338:	1a102737          	lui	a4,0x1a102
1c00b33c:	4314                	lw	a3,0(a4)
1c00b33e:	4605                	li	a2,1
1c00b340:	1a1065b7          	lui	a1,0x1a106
1c00b344:	009614b3          	sll	s1,a2,s1
1c00b348:	40545793          	srai	a5,s0,0x5
1c00b34c:	00458813          	addi	a6,a1,4 # 1a106004 <__l1_end+0xa105fe4>
1c00b350:	8cd5                	or	s1,s1,a3
1c00b352:	078a                	slli	a5,a5,0x2
1c00b354:	0a97222b          	p.sw	s1,164(a4!) # 1a1020a4 <__l1_end+0xa102084>
1c00b358:	010788b3          	add	a7,a5,a6
1c00b35c:	0008a503          	lw	a0,0(a7)
1c00b360:	f4543433          	p.bclr	s0,s0,26,5
1c00b364:	00861433          	sll	s0,a2,s0
1c00b368:	fff44413          	not	s0,s0
1c00b36c:	405b5693          	srai	a3,s6,0x5
1c00b370:	8d61                	and	a0,a0,s0
1c00b372:	068a                	slli	a3,a3,0x2
1c00b374:	00a8a023          	sw	a0,0(a7)
1c00b378:	96c2                	add	a3,a3,a6
1c00b37a:	4288                	lw	a0,0(a3)
1c00b37c:	01661633          	sll	a2,a2,s6
1c00b380:	fff64613          	not	a2,a2
1c00b384:	8e69                	and	a2,a2,a0
1c00b386:	04458593          	addi	a1,a1,68
1c00b38a:	c290                	sw	a2,0(a3)
1c00b38c:	97ae                	add	a5,a5,a1
1c00b38e:	4394                	lw	a3,0(a5)
1c00b390:	8c75                	and	s0,s0,a3
1c00b392:	c380                	sw	s0,0(a5)
1c00b394:	008aa603          	lw	a2,8(s5)
1c00b398:	00802583          	lw	a1,8(zero) # 8 <pos_cluster_pool>
1c00b39c:	00165793          	srli	a5,a2,0x1
1c00b3a0:	004aa683          	lw	a3,4(s5)
1c00b3a4:	97ae                	add	a5,a5,a1
1c00b3a6:	02c7d7b3          	divu	a5,a5,a2
1c00b3aa:	069e                	slli	a3,a3,0x7
1c00b3ac:	9736                	add	a4,a4,a3
1c00b3ae:	17fd                	addi	a5,a5,-1
1c00b3b0:	07c2                	slli	a5,a5,0x10
1c00b3b2:	3067e793          	ori	a5,a5,774
1c00b3b6:	c31c                	sw	a5,0(a4)
1c00b3b8:	30099073          	csrw	mstatus,s3
1c00b3bc:	50b2                	lw	ra,44(sp)
1c00b3be:	5422                	lw	s0,40(sp)
1c00b3c0:	5492                	lw	s1,36(sp)
1c00b3c2:	5902                	lw	s2,32(sp)
1c00b3c4:	49f2                	lw	s3,28(sp)
1c00b3c6:	4a62                	lw	s4,24(sp)
1c00b3c8:	4ad2                	lw	s5,20(sp)
1c00b3ca:	4b42                	lw	s6,16(sp)
1c00b3cc:	4bb2                	lw	s7,12(sp)
1c00b3ce:	4c22                	lw	s8,8(sp)
1c00b3d0:	4c92                	lw	s9,4(sp)
1c00b3d2:	4501                	li	a0,0
1c00b3d4:	6145                	addi	sp,sp,48
1c00b3d6:	8082                	ret
1c00b3d8:	00149c93          	slli	s9,s1,0x1
1c00b3dc:	024b8513          	addi	a0,s7,36
1c00b3e0:	8622                	mv	a2,s0
1c00b3e2:	85e6                	mv	a1,s9
1c00b3e4:	954a                	add	a0,a0,s2
1c00b3e6:	22b5                	jal	1c00b552 <pos_udma_create_channel>
1c00b3e8:	010b8513          	addi	a0,s7,16
1c00b3ec:	865a                	mv	a2,s6
1c00b3ee:	001c8593          	addi	a1,s9,1
1c00b3f2:	954a                	add	a0,a0,s2
1c00b3f4:	2ab9                	jal	1c00b552 <pos_udma_create_channel>
1c00b3f6:	000aa703          	lw	a4,0(s5)
1c00b3fa:	b735                	j	1c00b326 <pi_uart_open+0x4e>

1c00b3fc <pi_uart_close>:
1c00b3fc:	30047873          	csrrci	a6,mstatus,8
1c00b400:	4518                	lw	a4,8(a0)
1c00b402:	1a1026b7          	lui	a3,0x1a102
1c00b406:	09868693          	addi	a3,a3,152 # 1a102098 <__l1_end+0xa102078>
1c00b40a:	4310                	lw	a2,0(a4)
1c00b40c:	435c                	lw	a5,4(a4)
1c00b40e:	167d                	addi	a2,a2,-1
1c00b410:	079e                	slli	a5,a5,0x7
1c00b412:	c310                	sw	a2,0(a4)
1c00b414:	00d78633          	add	a2,a5,a3
1c00b418:	4210                	lw	a2,0(a2)
1c00b41a:	8a41                	andi	a2,a2,16
1c00b41c:	ca11                	beqz	a2,1c00b430 <pi_uart_close+0x34>
1c00b41e:	10500073          	wfi
1c00b422:	435c                	lw	a5,4(a4)
1c00b424:	079e                	slli	a5,a5,0x7
1c00b426:	00d78633          	add	a2,a5,a3
1c00b42a:	4210                	lw	a2,0(a2)
1c00b42c:	8a41                	andi	a2,a2,16
1c00b42e:	fa65                	bnez	a2,1c00b41e <pi_uart_close+0x22>
1c00b430:	1a1026b7          	lui	a3,0x1a102
1c00b434:	0a068693          	addi	a3,a3,160 # 1a1020a0 <__l1_end+0xa102080>
1c00b438:	97b6                	add	a5,a5,a3
1c00b43a:	4394                	lw	a3,0(a5)
1c00b43c:	fc16b6b3          	p.bclr	a3,a3,30,1
1c00b440:	feed                	bnez	a3,1c00b43a <pi_uart_close+0x3e>
1c00b442:	f14025f3          	csrr	a1,mhartid
1c00b446:	1a10a637          	lui	a2,0x1a10a
1c00b44a:	80460893          	addi	a7,a2,-2044 # 1a109804 <__l1_end+0xa1097e4>
1c00b44e:	80860513          	addi	a0,a2,-2040
1c00b452:	03200693          	li	a3,50
1c00b456:	ca5595b3          	p.extractu	a1,a1,5,5
1c00b45a:	4e7d                	li	t3,31
1c00b45c:	6791                	lui	a5,0x4
1c00b45e:	00204337          	lui	t1,0x204
1c00b462:	81460613          	addi	a2,a2,-2028
1c00b466:	05c58263          	beq	a1,t3,1c00b4aa <pi_uart_close+0xae>
1c00b46a:	02f32423          	sw	a5,40(t1) # 204028 <__l1_heap_size+0x1c4048>
1c00b46e:	00f8a023          	sw	a5,0(a7)
1c00b472:	10500073          	wfi
1c00b476:	c11c                	sw	a5,0(a0)
1c00b478:	16fd                	addi	a3,a3,-1
1c00b47a:	f6f5                	bnez	a3,1c00b466 <pi_uart_close+0x6a>
1c00b47c:	435c                	lw	a5,4(a4)
1c00b47e:	1a1026b7          	lui	a3,0x1a102
1c00b482:	0a468613          	addi	a2,a3,164 # 1a1020a4 <__l1_end+0xa102084>
1c00b486:	079e                	slli	a5,a5,0x7
1c00b488:	97b2                	add	a5,a5,a2
1c00b48a:	00500637          	lui	a2,0x500
1c00b48e:	0619                	addi	a2,a2,6
1c00b490:	c390                	sw	a2,0(a5)
1c00b492:	4350                	lw	a2,4(a4)
1c00b494:	4785                	li	a5,1
1c00b496:	4298                	lw	a4,0(a3)
1c00b498:	00c797b3          	sll	a5,a5,a2
1c00b49c:	fff7c793          	not	a5,a5
1c00b4a0:	8ff9                	and	a5,a5,a4
1c00b4a2:	c29c                	sw	a5,0(a3)
1c00b4a4:	30081073          	csrw	mstatus,a6
1c00b4a8:	8082                	ret
1c00b4aa:	c21c                	sw	a5,0(a2)
1c00b4ac:	b7c9                	j	1c00b46e <pi_uart_close+0x72>

1c00b4ae <pi_uart_write>:
1c00b4ae:	4508                	lw	a0,8(a0)
1c00b4b0:	715d                	addi	sp,sp,-80
1c00b4b2:	1c00b7b7          	lui	a5,0x1c00b
1c00b4b6:	86b2                	mv	a3,a2
1c00b4b8:	f7478793          	addi	a5,a5,-140 # 1c00af74 <pos_task_handle_blocking>
1c00b4bc:	862e                	mv	a2,a1
1c00b4be:	4701                	li	a4,0
1c00b4c0:	858a                	mv	a1,sp
1c00b4c2:	0541                	addi	a0,a0,16
1c00b4c4:	c686                	sw	ra,76(sp)
1c00b4c6:	c4a2                	sw	s0,72(sp)
1c00b4c8:	c23e                	sw	a5,4(sp)
1c00b4ca:	c40a                	sw	sp,8(sp)
1c00b4cc:	00010a23          	sb	zero,20(sp)
1c00b4d0:	20c9                	jal	1c00b592 <pos_udma_enqueue>
1c00b4d2:	30047473          	csrrci	s0,mstatus,8
1c00b4d6:	01410783          	lb	a5,20(sp)
1c00b4da:	e789                	bnez	a5,1c00b4e4 <pi_uart_write+0x36>
1c00b4dc:	3445                	jal	1c00af7c <pos_task_handle>
1c00b4de:	01410783          	lb	a5,20(sp)
1c00b4e2:	dfed                	beqz	a5,1c00b4dc <pi_uart_write+0x2e>
1c00b4e4:	30041073          	csrw	mstatus,s0
1c00b4e8:	40b6                	lw	ra,76(sp)
1c00b4ea:	4426                	lw	s0,72(sp)
1c00b4ec:	4501                	li	a0,0
1c00b4ee:	6161                	addi	sp,sp,80
1c00b4f0:	8082                	ret

1c00b4f2 <pos_uart_init>:
1c00b4f2:	1c0107b7          	lui	a5,0x1c010
1c00b4f6:	3a07aa23          	sw	zero,948(a5) # 1c0103b4 <pos_uart>
1c00b4fa:	8082                	ret

1c00b4fc <pos_udma_handle_copy>:
1c00b4fc:	41d4                	lw	a3,4(a1)
1c00b4fe:	459c                	lw	a5,8(a1)
1c00b500:	4198                	lw	a4,0(a1)
1c00b502:	c194                	sw	a3,0(a1)
1c00b504:	cb9d                	beqz	a5,1c00b53a <pos_udma_handle_copy+0x3e>
1c00b506:	0007a803          	lw	a6,0(a5)
1c00b50a:	4994                	lw	a3,16(a1)
1c00b50c:	4fc8                	lw	a0,28(a5)
1c00b50e:	5390                	lw	a2,32(a5)
1c00b510:	c1dc                	sw	a5,4(a1)
1c00b512:	0105a423          	sw	a6,8(a1)
1c00b516:	87b6                	mv	a5,a3
1c00b518:	00a7a22b          	p.sw	a0,4(a5!)
1c00b51c:	c390                	sw	a2,0(a5)
1c00b51e:	06a1                	addi	a3,a3,8
1c00b520:	47c1                	li	a5,16
1c00b522:	c29c                	sw	a5,0(a3)
1c00b524:	01c02683          	lw	a3,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b528:	00072023          	sw	zero,0(a4)
1c00b52c:	ce91                	beqz	a3,1c00b548 <pos_udma_handle_copy+0x4c>
1c00b52e:	02002683          	lw	a3,32(zero) # 20 <pos_sched_last>
1c00b532:	c298                	sw	a4,0(a3)
1c00b534:	02e02023          	sw	a4,32(zero) # 20 <pos_sched_last>
1c00b538:	8082                	ret
1c00b53a:	01c02683          	lw	a3,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b53e:	0005a223          	sw	zero,4(a1)
1c00b542:	00072023          	sw	zero,0(a4)
1c00b546:	f6e5                	bnez	a3,1c00b52e <pos_udma_handle_copy+0x32>
1c00b548:	00e02e23          	sw	a4,28(zero) # 1c <pos_cluster_nb_active_pe>
1c00b54c:	02e02023          	sw	a4,32(zero) # 20 <pos_sched_last>
1c00b550:	8082                	ret

1c00b552 <pos_udma_create_channel>:
1c00b552:	060a                	slli	a2,a2,0x2
1c00b554:	44400693          	li	a3,1092
1c00b558:	04400713          	li	a4,68
1c00b55c:	4015d793          	srai	a5,a1,0x1
1c00b560:	96b2                	add	a3,a3,a2
1c00b562:	9732                	add	a4,a4,a2
1c00b564:	1a102637          	lui	a2,0x1a102
1c00b568:	08060613          	addi	a2,a2,128 # 1a102080 <__l1_end+0xa102060>
1c00b56c:	079e                	slli	a5,a5,0x7
1c00b56e:	97b2                	add	a5,a5,a2
1c00b570:	0592                	slli	a1,a1,0x4
1c00b572:	1c00b637          	lui	a2,0x1c00b
1c00b576:	89c1                	andi	a1,a1,16
1c00b578:	4fc60613          	addi	a2,a2,1276 # 1c00b4fc <pos_udma_handle_copy>
1c00b57c:	c290                	sw	a2,0(a3)
1c00b57e:	c308                	sw	a0,0(a4)
1c00b580:	95be                	add	a1,a1,a5
1c00b582:	00052023          	sw	zero,0(a0)
1c00b586:	00052223          	sw	zero,4(a0)
1c00b58a:	00052423          	sw	zero,8(a0)
1c00b58e:	c90c                	sw	a1,16(a0)
1c00b590:	8082                	ret

1c00b592 <pos_udma_enqueue>:
1c00b592:	411c                	lw	a5,0(a0)
1c00b594:	cb8d                	beqz	a5,1c00b5c6 <pos_udma_enqueue+0x34>
1c00b596:	415c                	lw	a5,4(a0)
1c00b598:	cf81                	beqz	a5,1c00b5b0 <pos_udma_enqueue+0x1e>
1c00b59a:	451c                	lw	a5,8(a0)
1c00b59c:	cdd0                	sw	a2,28(a1)
1c00b59e:	d194                	sw	a3,32(a1)
1c00b5a0:	d1d8                	sw	a4,36(a1)
1c00b5a2:	cf8d                	beqz	a5,1c00b5dc <pos_udma_enqueue+0x4a>
1c00b5a4:	455c                	lw	a5,12(a0)
1c00b5a6:	c38c                	sw	a1,0(a5)
1c00b5a8:	c54c                	sw	a1,12(a0)
1c00b5aa:	0005a023          	sw	zero,0(a1)
1c00b5ae:	8082                	ret
1c00b5b0:	491c                	lw	a5,16(a0)
1c00b5b2:	c14c                	sw	a1,4(a0)
1c00b5b4:	c0474733          	p.bset	a4,a4,0,4
1c00b5b8:	85be                	mv	a1,a5
1c00b5ba:	00c5a22b          	p.sw	a2,4(a1!)
1c00b5be:	c194                	sw	a3,0(a1)
1c00b5c0:	07a1                	addi	a5,a5,8
1c00b5c2:	c398                	sw	a4,0(a5)
1c00b5c4:	8082                	ret
1c00b5c6:	491c                	lw	a5,16(a0)
1c00b5c8:	c10c                	sw	a1,0(a0)
1c00b5ca:	c0474733          	p.bset	a4,a4,0,4
1c00b5ce:	85be                	mv	a1,a5
1c00b5d0:	00c5a22b          	p.sw	a2,4(a1!)
1c00b5d4:	c194                	sw	a3,0(a1)
1c00b5d6:	07a1                	addi	a5,a5,8
1c00b5d8:	c398                	sw	a4,0(a5)
1c00b5da:	8082                	ret
1c00b5dc:	c50c                	sw	a1,8(a0)
1c00b5de:	c54c                	sw	a1,12(a0)
1c00b5e0:	0005a023          	sw	zero,0(a1)
1c00b5e4:	8082                	ret

1c00b5e6 <pos_cluster_push_fc_event>:
1c00b5e6:	00204637          	lui	a2,0x204
1c00b5ea:	0c060613          	addi	a2,a2,192 # 2040c0 <__l1_heap_size+0x1c40e0>
1c00b5ee:	421c                	lw	a5,0(a2)
1c00b5f0:	f1402373          	csrr	t1,mhartid
1c00b5f4:	1c0018b7          	lui	a7,0x1c001
1c00b5f8:	42888893          	addi	a7,a7,1064 # 1c001428 <pos_cluster>
1c00b5fc:	7e037313          	andi	t1,t1,2016
1c00b600:	006885b3          	add	a1,a7,t1
1c00b604:	49dc                	lw	a5,20(a1)
1c00b606:	c38d                	beqz	a5,1c00b628 <pos_cluster_push_fc_event+0x42>
1c00b608:	002047b7          	lui	a5,0x204
1c00b60c:	4709                	li	a4,2
1c00b60e:	03c78813          	addi	a6,a5,60 # 20403c <__l1_heap_size+0x1c405c>
1c00b612:	0c07a023          	sw	zero,192(a5)
1c00b616:	00e7a423          	sw	a4,8(a5)
1c00b61a:	00082683          	lw	a3,0(a6)
1c00b61e:	00e7a223          	sw	a4,4(a5)
1c00b622:	4214                	lw	a3,0(a2)
1c00b624:	49d4                	lw	a3,20(a1)
1c00b626:	f6f5                	bnez	a3,1c00b612 <pos_cluster_push_fc_event+0x2c>
1c00b628:	989a                	add	a7,a7,t1
1c00b62a:	1a10a7b7          	lui	a5,0x1a10a
1c00b62e:	00a8aa23          	sw	a0,20(a7)
1c00b632:	4709                	li	a4,2
1c00b634:	80e7a823          	sw	a4,-2032(a5) # 1a109810 <__l1_end+0xa1097f0>
1c00b638:	002047b7          	lui	a5,0x204
1c00b63c:	0c07a023          	sw	zero,192(a5) # 2040c0 <__l1_heap_size+0x1c40e0>
1c00b640:	8082                	ret

1c00b642 <pos_init_entry>:
1c00b642:	7a101073          	csrw	tdata1,zero
1c00b646:	f1402573          	csrr	a0,mhartid
1c00b64a:	01f57593          	andi	a1,a0,31
1c00b64e:	8115                	srli	a0,a0,0x5
1c00b650:	467d                	li	a2,31
1c00b652:	00c50463          	beq	a0,a2,1c00b65a <pos_init_entry+0x18>
1c00b656:	1ab0406f          	j	1c010000 <__cluster_text_start>
1c00b65a:	ffff6297          	auipc	t0,0xffff6
1c00b65e:	db628293          	addi	t0,t0,-586 # 1c001410 <pos_io_uart_enabled>
1c00b662:	ffff6317          	auipc	t1,0xffff6
1c00b666:	de630313          	addi	t1,t1,-538 # 1c001448 <__l2_priv0_end>
1c00b66a:	0002a023          	sw	zero,0(t0)
1c00b66e:	0291                	addi	t0,t0,4
1c00b670:	fe62ede3          	bltu	t0,t1,1c00b66a <pos_init_entry+0x28>
1c00b674:	ffff6117          	auipc	sp,0xffff6
1c00b678:	d5c10113          	addi	sp,sp,-676 # 1c0013d0 <pos_cbsys_first>
1c00b67c:	00000297          	auipc	t0,0x0
1c00b680:	87a28293          	addi	t0,t0,-1926 # 1c00aef6 <pos_init_start>
1c00b684:	000280e7          	jalr	t0
1c00b688:	00000513          	li	a0,0
1c00b68c:	00000593          	li	a1,0
1c00b690:	ffffd397          	auipc	t2,0xffffd
1c00b694:	55a38393          	addi	t2,t2,1370 # 1c008bea <main>
1c00b698:	000380e7          	jalr	t2
1c00b69c:	842a                	mv	s0,a0
1c00b69e:	8522                	mv	a0,s0
1c00b6a0:	ffffd297          	auipc	t0,0xffffd
1c00b6a4:	78028293          	addi	t0,t0,1920 # 1c008e20 <exit>
1c00b6a8:	000280e7          	jalr	t0

1c00b6ac <pos_irq_call_external_c_function>:
1c00b6ac:	7119                	addi	sp,sp,-128
1c00b6ae:	c006                	sw	ra,0(sp)
1c00b6b0:	c20e                	sw	gp,4(sp)
1c00b6b2:	c412                	sw	tp,8(sp)
1c00b6b4:	c616                	sw	t0,12(sp)
1c00b6b6:	c81a                	sw	t1,16(sp)
1c00b6b8:	ca1e                	sw	t2,20(sp)
1c00b6ba:	d236                	sw	a3,36(sp)
1c00b6bc:	d43a                	sw	a4,40(sp)
1c00b6be:	d63e                	sw	a5,44(sp)
1c00b6c0:	d842                	sw	a6,48(sp)
1c00b6c2:	da46                	sw	a7,52(sp)
1c00b6c4:	dc72                	sw	t3,56(sp)
1c00b6c6:	de76                	sw	t4,60(sp)
1c00b6c8:	c0fa                	sw	t5,64(sp)
1c00b6ca:	c6fe                	sw	t6,76(sp)
1c00b6cc:	000600e7          	jalr	a2
1c00b6d0:	4082                	lw	ra,0(sp)
1c00b6d2:	4192                	lw	gp,4(sp)
1c00b6d4:	4222                	lw	tp,8(sp)
1c00b6d6:	42b2                	lw	t0,12(sp)
1c00b6d8:	4342                	lw	t1,16(sp)
1c00b6da:	43d2                	lw	t2,20(sp)
1c00b6dc:	5692                	lw	a3,36(sp)
1c00b6de:	5722                	lw	a4,40(sp)
1c00b6e0:	57b2                	lw	a5,44(sp)
1c00b6e2:	5842                	lw	a6,48(sp)
1c00b6e4:	58d2                	lw	a7,52(sp)
1c00b6e6:	5e62                	lw	t3,56(sp)
1c00b6e8:	5ef2                	lw	t4,60(sp)
1c00b6ea:	4f06                	lw	t5,64(sp)
1c00b6ec:	4fb6                	lw	t6,76(sp)
1c00b6ee:	6109                	addi	sp,sp,128
1c00b6f0:	8482                	jr	s1

1c00b6f2 <pos_irq_call_external_c_function_full>:
1c00b6f2:	7119                	addi	sp,sp,-128
1c00b6f4:	c006                	sw	ra,0(sp)
1c00b6f6:	c20e                	sw	gp,4(sp)
1c00b6f8:	c412                	sw	tp,8(sp)
1c00b6fa:	c616                	sw	t0,12(sp)
1c00b6fc:	c81a                	sw	t1,16(sp)
1c00b6fe:	ca1e                	sw	t2,20(sp)
1c00b700:	cc2a                	sw	a0,24(sp)
1c00b702:	ce2e                	sw	a1,28(sp)
1c00b704:	d236                	sw	a3,36(sp)
1c00b706:	d43a                	sw	a4,40(sp)
1c00b708:	d63e                	sw	a5,44(sp)
1c00b70a:	d842                	sw	a6,48(sp)
1c00b70c:	da46                	sw	a7,52(sp)
1c00b70e:	dc72                	sw	t3,56(sp)
1c00b710:	de76                	sw	t4,60(sp)
1c00b712:	c0fa                	sw	t5,64(sp)
1c00b714:	c6fe                	sw	t6,76(sp)
1c00b716:	000600e7          	jalr	a2
1c00b71a:	4082                	lw	ra,0(sp)
1c00b71c:	4192                	lw	gp,4(sp)
1c00b71e:	4222                	lw	tp,8(sp)
1c00b720:	42b2                	lw	t0,12(sp)
1c00b722:	4342                	lw	t1,16(sp)
1c00b724:	43d2                	lw	t2,20(sp)
1c00b726:	4562                	lw	a0,24(sp)
1c00b728:	45f2                	lw	a1,28(sp)
1c00b72a:	5692                	lw	a3,36(sp)
1c00b72c:	5722                	lw	a4,40(sp)
1c00b72e:	57b2                	lw	a5,44(sp)
1c00b730:	5842                	lw	a6,48(sp)
1c00b732:	58d2                	lw	a7,52(sp)
1c00b734:	5e62                	lw	t3,56(sp)
1c00b736:	5ef2                	lw	t4,60(sp)
1c00b738:	4f06                	lw	t5,64(sp)
1c00b73a:	4fb6                	lw	t6,76(sp)
1c00b73c:	6109                	addi	sp,sp,128
1c00b73e:	8482                	jr	s1

1c00b740 <pos_time_timer_handler_asm>:
1c00b740:	1161                	addi	sp,sp,-8
1c00b742:	c032                	sw	a2,0(sp)
1c00b744:	c226                	sw	s1,4(sp)
1c00b746:	00000617          	auipc	a2,0x0
1c00b74a:	a4460613          	addi	a2,a2,-1468 # 1c00b18a <pos_time_timer_handler>
1c00b74e:	00000497          	auipc	s1,0x0
1c00b752:	00c48493          	addi	s1,s1,12 # 1c00b75a <pos_time_timer_handler_asm_ret>
1c00b756:	f9dff06f          	j	1c00b6f2 <pos_irq_call_external_c_function_full>

1c00b75a <pos_time_timer_handler_asm_ret>:
1c00b75a:	4492                	lw	s1,4(sp)
1c00b75c:	4602                	lw	a2,0(sp)
1c00b75e:	0121                	addi	sp,sp,8
1c00b760:	30200073          	mret

1c00b764 <pos_soc_event_handler_asm>:
1c00b764:	7119                	addi	sp,sp,-128
1c00b766:	c022                	sw	s0,0(sp)
1c00b768:	c226                	sw	s1,4(sp)
1c00b76a:	c42a                	sw	a0,8(sp)
1c00b76c:	c62e                	sw	a1,12(sp)
1c00b76e:	c832                	sw	a2,16(sp)
1c00b770:	1a10a437          	lui	s0,0x1a10a
1c00b774:	80040413          	addi	s0,s0,-2048 # 1a109800 <__l1_end+0xa1097e0>
1c00b778:	5048                	lw	a0,36(s0)
1c00b77a:	00251593          	slli	a1,a0,0x2
1c00b77e:	44400293          	li	t0,1092
1c00b782:	2055f603          	p.lw	a2,t0(a1)
1c00b786:	04400293          	li	t0,68
1c00b78a:	2055f583          	p.lw	a1,t0(a1)
1c00b78e:	00000497          	auipc	s1,0x0
1c00b792:	02848493          	addi	s1,s1,40 # 1c00b7b6 <pos_soc_event_handler_end_asm>
1c00b796:	f17ff06f          	j	1c00b6ac <pos_irq_call_external_c_function>

1c00b79a <pos_soc_event_no_udma_asm>:
1c00b79a:	00555593          	srli	a1,a0,0x5
1c00b79e:	058a                	slli	a1,a1,0x2
1c00b7a0:	02400293          	li	t0,36
1c00b7a4:	2055f603          	p.lw	a2,t0(a1)
1c00b7a8:	897d                	andi	a0,a0,31
1c00b7aa:	80a64633          	p.bsetr	a2,a2,a0
1c00b7ae:	02400293          	li	t0,36
1c00b7b2:	00c5e2a3          	p.sw	a2,t0(a1)

1c00b7b6 <pos_soc_event_handler_end_asm>:
1c00b7b6:	4402                	lw	s0,0(sp)
1c00b7b8:	4492                	lw	s1,4(sp)
1c00b7ba:	4522                	lw	a0,8(sp)
1c00b7bc:	45b2                	lw	a1,12(sp)
1c00b7be:	4642                	lw	a2,16(sp)
1c00b7c0:	6109                	addi	sp,sp,128
1c00b7c2:	30200073          	mret

1c00b7c6 <_endtext>:
	...

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
1c01003e:	3eeb8b93          	addi	s7,s7,1006 # 1c001428 <pos_cluster>
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
