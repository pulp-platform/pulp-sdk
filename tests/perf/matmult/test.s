riscv32-unknown-elf-objdump -March=rv32imcxgap9 -d /home/bruschin/bruschin/Training/pulp-sdk/tests/perf/matmult/BUILD/PULPISSIMO/GCC_RISCV//test/test

/home/bruschin/bruschin/Training/pulp-sdk/tests/perf/matmult/BUILD/PULPISSIMO/GCC_RISCV//test/test:     file format elf32-littleriscv


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
1c008084:	8de50513          	addi	a0,a0,-1826 # 1c00a95e <pos_init_entry>
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
1c00809c:	87b2                	mv	a5,a2
1c00809e:	8736                	mv	a4,a3
1c0080a0:	88aa                	mv	a7,a0
1c0080a2:	882e                	mv	a6,a1
1c0080a4:	1e069d63          	bnez	a3,1c00829e <__udivdi3+0x202>
1c0080a8:	1c001337          	lui	t1,0x1c001
1c0080ac:	86030313          	addi	t1,t1,-1952 # 1c000860 <__DTOR_END__>
1c0080b0:	0ac5fd63          	bleu	a2,a1,1c00816a <__udivdi3+0xce>
1c0080b4:	6741                	lui	a4,0x10
1c0080b6:	0ae67363          	bleu	a4,a2,1c00815c <__udivdi3+0xc0>
1c0080ba:	0ff00693          	li	a3,255
1c0080be:	00c6b6b3          	sltu	a3,a3,a2
1c0080c2:	068e                	slli	a3,a3,0x3
1c0080c4:	00d65733          	srl	a4,a2,a3
1c0080c8:	933a                	add	t1,t1,a4
1c0080ca:	00034703          	lbu	a4,0(t1)
1c0080ce:	02000313          	li	t1,32
1c0080d2:	96ba                	add	a3,a3,a4
1c0080d4:	40d30333          	sub	t1,t1,a3
1c0080d8:	00030c63          	beqz	t1,1c0080f0 <__udivdi3+0x54>
1c0080dc:	00659733          	sll	a4,a1,t1
1c0080e0:	00d556b3          	srl	a3,a0,a3
1c0080e4:	006617b3          	sll	a5,a2,t1
1c0080e8:	00e6e833          	or	a6,a3,a4
1c0080ec:	006518b3          	sll	a7,a0,t1
1c0080f0:	0107d513          	srli	a0,a5,0x10
1c0080f4:	02a85733          	divu	a4,a6,a0
1c0080f8:	1007d5b3          	p.exthz	a1,a5
1c0080fc:	0108d693          	srli	a3,a7,0x10
1c008100:	02a87633          	remu	a2,a6,a0
1c008104:	02e58833          	mul	a6,a1,a4
1c008108:	0642                	slli	a2,a2,0x10
1c00810a:	8ed1                	or	a3,a3,a2
1c00810c:	863a                	mv	a2,a4
1c00810e:	0106fc63          	bleu	a6,a3,1c008126 <__udivdi3+0x8a>
1c008112:	96be                	add	a3,a3,a5
1c008114:	fff70613          	addi	a2,a4,-1 # ffff <__l1_heap_size+0x3>
1c008118:	00f6e763          	bltu	a3,a5,1c008126 <__udivdi3+0x8a>
1c00811c:	0106f563          	bleu	a6,a3,1c008126 <__udivdi3+0x8a>
1c008120:	ffe70613          	addi	a2,a4,-2
1c008124:	96be                	add	a3,a3,a5
1c008126:	410686b3          	sub	a3,a3,a6
1c00812a:	02a6f833          	remu	a6,a3,a0
1c00812e:	02a6d6b3          	divu	a3,a3,a0
1c008132:	df0828b3          	p.insert	a7,a6,15,16
1c008136:	02d58733          	mul	a4,a1,a3
1c00813a:	8536                	mv	a0,a3
1c00813c:	00e8fb63          	bleu	a4,a7,1c008152 <__udivdi3+0xb6>
1c008140:	98be                	add	a7,a7,a5
1c008142:	fff68513          	addi	a0,a3,-1
1c008146:	00f8e663          	bltu	a7,a5,1c008152 <__udivdi3+0xb6>
1c00814a:	00e8f463          	bleu	a4,a7,1c008152 <__udivdi3+0xb6>
1c00814e:	ffe68513          	addi	a0,a3,-2
1c008152:	01061793          	slli	a5,a2,0x10
1c008156:	8fc9                	or	a5,a5,a0
1c008158:	4801                	li	a6,0
1c00815a:	a06d                	j	1c008204 <__udivdi3+0x168>
1c00815c:	01000737          	lui	a4,0x1000
1c008160:	46c1                	li	a3,16
1c008162:	f6e661e3          	bltu	a2,a4,1c0080c4 <__udivdi3+0x28>
1c008166:	46e1                	li	a3,24
1c008168:	bfb1                	j	1c0080c4 <__udivdi3+0x28>
1c00816a:	e601                	bnez	a2,1c008172 <__udivdi3+0xd6>
1c00816c:	4685                	li	a3,1
1c00816e:	02c6d7b3          	divu	a5,a3,a2
1c008172:	66c1                	lui	a3,0x10
1c008174:	08d7fb63          	bleu	a3,a5,1c00820a <__udivdi3+0x16e>
1c008178:	0ff00693          	li	a3,255
1c00817c:	00f6f363          	bleu	a5,a3,1c008182 <__udivdi3+0xe6>
1c008180:	4721                	li	a4,8
1c008182:	00e7d6b3          	srl	a3,a5,a4
1c008186:	9336                	add	t1,t1,a3
1c008188:	00034683          	lbu	a3,0(t1)
1c00818c:	02000613          	li	a2,32
1c008190:	96ba                	add	a3,a3,a4
1c008192:	8e15                	sub	a2,a2,a3
1c008194:	e251                	bnez	a2,1c008218 <__udivdi3+0x17c>
1c008196:	40f58733          	sub	a4,a1,a5
1c00819a:	4805                	li	a6,1
1c00819c:	0107d513          	srli	a0,a5,0x10
1c0081a0:	1007d5b3          	p.exthz	a1,a5
1c0081a4:	0108d693          	srli	a3,a7,0x10
1c0081a8:	02a77633          	remu	a2,a4,a0
1c0081ac:	02a75733          	divu	a4,a4,a0
1c0081b0:	0642                	slli	a2,a2,0x10
1c0081b2:	8ed1                	or	a3,a3,a2
1c0081b4:	02e58333          	mul	t1,a1,a4
1c0081b8:	863a                	mv	a2,a4
1c0081ba:	0066fc63          	bleu	t1,a3,1c0081d2 <__udivdi3+0x136>
1c0081be:	96be                	add	a3,a3,a5
1c0081c0:	fff70613          	addi	a2,a4,-1 # ffffff <__l1_heap_size+0xff0003>
1c0081c4:	00f6e763          	bltu	a3,a5,1c0081d2 <__udivdi3+0x136>
1c0081c8:	0066f563          	bleu	t1,a3,1c0081d2 <__udivdi3+0x136>
1c0081cc:	ffe70613          	addi	a2,a4,-2
1c0081d0:	96be                	add	a3,a3,a5
1c0081d2:	406686b3          	sub	a3,a3,t1
1c0081d6:	02a6f333          	remu	t1,a3,a0
1c0081da:	02a6d6b3          	divu	a3,a3,a0
1c0081de:	df0328b3          	p.insert	a7,t1,15,16
1c0081e2:	02d58733          	mul	a4,a1,a3
1c0081e6:	8536                	mv	a0,a3
1c0081e8:	00e8fb63          	bleu	a4,a7,1c0081fe <__udivdi3+0x162>
1c0081ec:	98be                	add	a7,a7,a5
1c0081ee:	fff68513          	addi	a0,a3,-1 # ffff <__l1_heap_size+0x3>
1c0081f2:	00f8e663          	bltu	a7,a5,1c0081fe <__udivdi3+0x162>
1c0081f6:	00e8f463          	bleu	a4,a7,1c0081fe <__udivdi3+0x162>
1c0081fa:	ffe68513          	addi	a0,a3,-2
1c0081fe:	01061793          	slli	a5,a2,0x10
1c008202:	8fc9                	or	a5,a5,a0
1c008204:	853e                	mv	a0,a5
1c008206:	85c2                	mv	a1,a6
1c008208:	8082                	ret
1c00820a:	010006b7          	lui	a3,0x1000
1c00820e:	4741                	li	a4,16
1c008210:	f6d7e9e3          	bltu	a5,a3,1c008182 <__udivdi3+0xe6>
1c008214:	4761                	li	a4,24
1c008216:	b7b5                	j	1c008182 <__udivdi3+0xe6>
1c008218:	00c797b3          	sll	a5,a5,a2
1c00821c:	00d5d333          	srl	t1,a1,a3
1c008220:	0107de13          	srli	t3,a5,0x10
1c008224:	00c59733          	sll	a4,a1,a2
1c008228:	00c518b3          	sll	a7,a0,a2
1c00822c:	00d555b3          	srl	a1,a0,a3
1c008230:	03c35533          	divu	a0,t1,t3
1c008234:	8dd9                	or	a1,a1,a4
1c008236:	1007d733          	p.exthz	a4,a5
1c00823a:	0105d693          	srli	a3,a1,0x10
1c00823e:	03c37633          	remu	a2,t1,t3
1c008242:	882a                	mv	a6,a0
1c008244:	02a70333          	mul	t1,a4,a0
1c008248:	0642                	slli	a2,a2,0x10
1c00824a:	8ed1                	or	a3,a3,a2
1c00824c:	0066fc63          	bleu	t1,a3,1c008264 <__udivdi3+0x1c8>
1c008250:	96be                	add	a3,a3,a5
1c008252:	fff50813          	addi	a6,a0,-1
1c008256:	00f6e763          	bltu	a3,a5,1c008264 <__udivdi3+0x1c8>
1c00825a:	0066f563          	bleu	t1,a3,1c008264 <__udivdi3+0x1c8>
1c00825e:	ffe50813          	addi	a6,a0,-2
1c008262:	96be                	add	a3,a3,a5
1c008264:	406686b3          	sub	a3,a3,t1
1c008268:	03c6f633          	remu	a2,a3,t3
1c00826c:	03c6d6b3          	divu	a3,a3,t3
1c008270:	df0625b3          	p.insert	a1,a2,15,16
1c008274:	02d70733          	mul	a4,a4,a3
1c008278:	8636                	mv	a2,a3
1c00827a:	00e5fc63          	bleu	a4,a1,1c008292 <__udivdi3+0x1f6>
1c00827e:	95be                	add	a1,a1,a5
1c008280:	fff68613          	addi	a2,a3,-1 # ffffff <__l1_heap_size+0xff0003>
1c008284:	00f5e763          	bltu	a1,a5,1c008292 <__udivdi3+0x1f6>
1c008288:	00e5f563          	bleu	a4,a1,1c008292 <__udivdi3+0x1f6>
1c00828c:	ffe68613          	addi	a2,a3,-2
1c008290:	95be                	add	a1,a1,a5
1c008292:	0842                	slli	a6,a6,0x10
1c008294:	40e58733          	sub	a4,a1,a4
1c008298:	00c86833          	or	a6,a6,a2
1c00829c:	b701                	j	1c00819c <__udivdi3+0x100>
1c00829e:	12d5ea63          	bltu	a1,a3,1c0083d2 <__udivdi3+0x336>
1c0082a2:	67c1                	lui	a5,0x10
1c0082a4:	02f6fd63          	bleu	a5,a3,1c0082de <__udivdi3+0x242>
1c0082a8:	0ff00793          	li	a5,255
1c0082ac:	00d7b8b3          	sltu	a7,a5,a3
1c0082b0:	088e                	slli	a7,a7,0x3
1c0082b2:	1c001737          	lui	a4,0x1c001
1c0082b6:	0116d7b3          	srl	a5,a3,a7
1c0082ba:	86070713          	addi	a4,a4,-1952 # 1c000860 <__DTOR_END__>
1c0082be:	97ba                	add	a5,a5,a4
1c0082c0:	0007c783          	lbu	a5,0(a5) # 10000 <__l1_heap_size+0x4>
1c0082c4:	02000813          	li	a6,32
1c0082c8:	97c6                	add	a5,a5,a7
1c0082ca:	40f80833          	sub	a6,a6,a5
1c0082ce:	00081f63          	bnez	a6,1c0082ec <__udivdi3+0x250>
1c0082d2:	4785                	li	a5,1
1c0082d4:	f2b6e8e3          	bltu	a3,a1,1c008204 <__udivdi3+0x168>
1c0082d8:	04a637b3          	p.sletu	a5,a2,a0
1c0082dc:	b725                	j	1c008204 <__udivdi3+0x168>
1c0082de:	010007b7          	lui	a5,0x1000
1c0082e2:	48c1                	li	a7,16
1c0082e4:	fcf6e7e3          	bltu	a3,a5,1c0082b2 <__udivdi3+0x216>
1c0082e8:	48e1                	li	a7,24
1c0082ea:	b7e1                	j	1c0082b2 <__udivdi3+0x216>
1c0082ec:	00f658b3          	srl	a7,a2,a5
1c0082f0:	010696b3          	sll	a3,a3,a6
1c0082f4:	00d8e6b3          	or	a3,a7,a3
1c0082f8:	00f5d333          	srl	t1,a1,a5
1c0082fc:	0106de13          	srli	t3,a3,0x10
1c008300:	00f55733          	srl	a4,a0,a5
1c008304:	03c377b3          	remu	a5,t1,t3
1c008308:	1006d8b3          	p.exthz	a7,a3
1c00830c:	010595b3          	sll	a1,a1,a6
1c008310:	8f4d                	or	a4,a4,a1
1c008312:	01075593          	srli	a1,a4,0x10
1c008316:	01061633          	sll	a2,a2,a6
1c00831a:	03c35333          	divu	t1,t1,t3
1c00831e:	07c2                	slli	a5,a5,0x10
1c008320:	8ddd                	or	a1,a1,a5
1c008322:	02688eb3          	mul	t4,a7,t1
1c008326:	879a                	mv	a5,t1
1c008328:	01d5fc63          	bleu	t4,a1,1c008340 <__udivdi3+0x2a4>
1c00832c:	95b6                	add	a1,a1,a3
1c00832e:	fff30793          	addi	a5,t1,-1
1c008332:	00d5e763          	bltu	a1,a3,1c008340 <__udivdi3+0x2a4>
1c008336:	01d5f563          	bleu	t4,a1,1c008340 <__udivdi3+0x2a4>
1c00833a:	ffe30793          	addi	a5,t1,-2
1c00833e:	95b6                	add	a1,a1,a3
1c008340:	41d585b3          	sub	a1,a1,t4
1c008344:	03c5f333          	remu	t1,a1,t3
1c008348:	03c5d5b3          	divu	a1,a1,t3
1c00834c:	df032733          	p.insert	a4,t1,15,16
1c008350:	02b888b3          	mul	a7,a7,a1
1c008354:	832e                	mv	t1,a1
1c008356:	01177c63          	bleu	a7,a4,1c00836e <__udivdi3+0x2d2>
1c00835a:	9736                	add	a4,a4,a3
1c00835c:	fff58313          	addi	t1,a1,-1
1c008360:	00d76763          	bltu	a4,a3,1c00836e <__udivdi3+0x2d2>
1c008364:	01177563          	bleu	a7,a4,1c00836e <__udivdi3+0x2d2>
1c008368:	ffe58313          	addi	t1,a1,-2
1c00836c:	9736                	add	a4,a4,a3
1c00836e:	07c2                	slli	a5,a5,0x10
1c008370:	6e41                	lui	t3,0x10
1c008372:	0067e7b3          	or	a5,a5,t1
1c008376:	fffe0593          	addi	a1,t3,-1 # ffff <__l1_heap_size+0x3>
1c00837a:	00b7f6b3          	and	a3,a5,a1
1c00837e:	41170733          	sub	a4,a4,a7
1c008382:	8df1                	and	a1,a1,a2
1c008384:	0107d893          	srli	a7,a5,0x10
1c008388:	02b68333          	mul	t1,a3,a1
1c00838c:	8241                	srli	a2,a2,0x10
1c00838e:	02b885b3          	mul	a1,a7,a1
1c008392:	8eae                	mv	t4,a1
1c008394:	42c68eb3          	p.mac	t4,a3,a2
1c008398:	01035693          	srli	a3,t1,0x10
1c00839c:	96f6                	add	a3,a3,t4
1c00839e:	02c888b3          	mul	a7,a7,a2
1c0083a2:	00b6f363          	bleu	a1,a3,1c0083a8 <__udivdi3+0x30c>
1c0083a6:	98f2                	add	a7,a7,t3
1c0083a8:	0106d613          	srli	a2,a3,0x10
1c0083ac:	98b2                	add	a7,a7,a2
1c0083ae:	03176063          	bltu	a4,a7,1c0083ce <__udivdi3+0x332>
1c0083b2:	db1713e3          	bne	a4,a7,1c008158 <__udivdi3+0xbc>
1c0083b6:	6741                	lui	a4,0x10
1c0083b8:	177d                	addi	a4,a4,-1
1c0083ba:	8ef9                	and	a3,a3,a4
1c0083bc:	06c2                	slli	a3,a3,0x10
1c0083be:	00e37333          	and	t1,t1,a4
1c0083c2:	01051533          	sll	a0,a0,a6
1c0083c6:	969a                	add	a3,a3,t1
1c0083c8:	4801                	li	a6,0
1c0083ca:	e2d57de3          	bleu	a3,a0,1c008204 <__udivdi3+0x168>
1c0083ce:	17fd                	addi	a5,a5,-1
1c0083d0:	b361                	j	1c008158 <__udivdi3+0xbc>
1c0083d2:	4801                	li	a6,0
1c0083d4:	4781                	li	a5,0
1c0083d6:	b53d                	j	1c008204 <__udivdi3+0x168>

1c0083d8 <__umoddi3>:
1c0083d8:	88b2                	mv	a7,a2
1c0083da:	8736                	mv	a4,a3
1c0083dc:	87aa                	mv	a5,a0
1c0083de:	882e                	mv	a6,a1
1c0083e0:	1a069963          	bnez	a3,1c008592 <__umoddi3+0x1ba>
1c0083e4:	1c0016b7          	lui	a3,0x1c001
1c0083e8:	86068693          	addi	a3,a3,-1952 # 1c000860 <__DTOR_END__>
1c0083ec:	0ac5f463          	bleu	a2,a1,1c008494 <__umoddi3+0xbc>
1c0083f0:	6341                	lui	t1,0x10
1c0083f2:	08667a63          	bleu	t1,a2,1c008486 <__umoddi3+0xae>
1c0083f6:	0ff00313          	li	t1,255
1c0083fa:	00c37363          	bleu	a2,t1,1c008400 <__umoddi3+0x28>
1c0083fe:	4721                	li	a4,8
1c008400:	00e65333          	srl	t1,a2,a4
1c008404:	969a                	add	a3,a3,t1
1c008406:	0006c683          	lbu	a3,0(a3)
1c00840a:	02000313          	li	t1,32
1c00840e:	9736                	add	a4,a4,a3
1c008410:	40e30333          	sub	t1,t1,a4
1c008414:	00030c63          	beqz	t1,1c00842c <__umoddi3+0x54>
1c008418:	006595b3          	sll	a1,a1,t1
1c00841c:	00e55733          	srl	a4,a0,a4
1c008420:	006618b3          	sll	a7,a2,t1
1c008424:	00b76833          	or	a6,a4,a1
1c008428:	006517b3          	sll	a5,a0,t1
1c00842c:	0108d613          	srli	a2,a7,0x10
1c008430:	02c87733          	remu	a4,a6,a2
1c008434:	1008d533          	p.exthz	a0,a7
1c008438:	0107d693          	srli	a3,a5,0x10
1c00843c:	02c85833          	divu	a6,a6,a2
1c008440:	0742                	slli	a4,a4,0x10
1c008442:	8ed9                	or	a3,a3,a4
1c008444:	03050833          	mul	a6,a0,a6
1c008448:	0106f863          	bleu	a6,a3,1c008458 <__umoddi3+0x80>
1c00844c:	96c6                	add	a3,a3,a7
1c00844e:	0116e563          	bltu	a3,a7,1c008458 <__umoddi3+0x80>
1c008452:	0106f363          	bleu	a6,a3,1c008458 <__umoddi3+0x80>
1c008456:	96c6                	add	a3,a3,a7
1c008458:	410686b3          	sub	a3,a3,a6
1c00845c:	02c6f733          	remu	a4,a3,a2
1c008460:	02c6d6b3          	divu	a3,a3,a2
1c008464:	df0727b3          	p.insert	a5,a4,15,16
1c008468:	02d506b3          	mul	a3,a0,a3
1c00846c:	00d7f863          	bleu	a3,a5,1c00847c <__umoddi3+0xa4>
1c008470:	97c6                	add	a5,a5,a7
1c008472:	0117e563          	bltu	a5,a7,1c00847c <__umoddi3+0xa4>
1c008476:	00d7f363          	bleu	a3,a5,1c00847c <__umoddi3+0xa4>
1c00847a:	97c6                	add	a5,a5,a7
1c00847c:	8f95                	sub	a5,a5,a3
1c00847e:	0067d533          	srl	a0,a5,t1
1c008482:	4581                	li	a1,0
1c008484:	8082                	ret
1c008486:	01000337          	lui	t1,0x1000
1c00848a:	4741                	li	a4,16
1c00848c:	f6666ae3          	bltu	a2,t1,1c008400 <__umoddi3+0x28>
1c008490:	4761                	li	a4,24
1c008492:	b7bd                	j	1c008400 <__umoddi3+0x28>
1c008494:	e601                	bnez	a2,1c00849c <__umoddi3+0xc4>
1c008496:	4605                	li	a2,1
1c008498:	031658b3          	divu	a7,a2,a7
1c00849c:	6641                	lui	a2,0x10
1c00849e:	06c8ff63          	bleu	a2,a7,1c00851c <__umoddi3+0x144>
1c0084a2:	0ff00613          	li	a2,255
1c0084a6:	01167363          	bleu	a7,a2,1c0084ac <__umoddi3+0xd4>
1c0084aa:	4721                	li	a4,8
1c0084ac:	00e8d633          	srl	a2,a7,a4
1c0084b0:	96b2                	add	a3,a3,a2
1c0084b2:	0006c603          	lbu	a2,0(a3)
1c0084b6:	02000313          	li	t1,32
1c0084ba:	963a                	add	a2,a2,a4
1c0084bc:	40c30333          	sub	t1,t1,a2
1c0084c0:	06031563          	bnez	t1,1c00852a <__umoddi3+0x152>
1c0084c4:	411585b3          	sub	a1,a1,a7
1c0084c8:	0108d713          	srli	a4,a7,0x10
1c0084cc:	1008d533          	p.exthz	a0,a7
1c0084d0:	0107d613          	srli	a2,a5,0x10
1c0084d4:	02e5f6b3          	remu	a3,a1,a4
1c0084d8:	02e5d5b3          	divu	a1,a1,a4
1c0084dc:	06c2                	slli	a3,a3,0x10
1c0084de:	8ed1                	or	a3,a3,a2
1c0084e0:	02b505b3          	mul	a1,a0,a1
1c0084e4:	00b6f863          	bleu	a1,a3,1c0084f4 <__umoddi3+0x11c>
1c0084e8:	96c6                	add	a3,a3,a7
1c0084ea:	0116e563          	bltu	a3,a7,1c0084f4 <__umoddi3+0x11c>
1c0084ee:	00b6f363          	bleu	a1,a3,1c0084f4 <__umoddi3+0x11c>
1c0084f2:	96c6                	add	a3,a3,a7
1c0084f4:	40b685b3          	sub	a1,a3,a1
1c0084f8:	02e5f6b3          	remu	a3,a1,a4
1c0084fc:	02e5d5b3          	divu	a1,a1,a4
1c008500:	df06a7b3          	p.insert	a5,a3,15,16
1c008504:	02b505b3          	mul	a1,a0,a1
1c008508:	00b7f863          	bleu	a1,a5,1c008518 <__umoddi3+0x140>
1c00850c:	97c6                	add	a5,a5,a7
1c00850e:	0117e563          	bltu	a5,a7,1c008518 <__umoddi3+0x140>
1c008512:	00b7f363          	bleu	a1,a5,1c008518 <__umoddi3+0x140>
1c008516:	97c6                	add	a5,a5,a7
1c008518:	8f8d                	sub	a5,a5,a1
1c00851a:	b795                	j	1c00847e <__umoddi3+0xa6>
1c00851c:	01000637          	lui	a2,0x1000
1c008520:	4741                	li	a4,16
1c008522:	f8c8e5e3          	bltu	a7,a2,1c0084ac <__umoddi3+0xd4>
1c008526:	4761                	li	a4,24
1c008528:	b751                	j	1c0084ac <__umoddi3+0xd4>
1c00852a:	006898b3          	sll	a7,a7,t1
1c00852e:	00c5d733          	srl	a4,a1,a2
1c008532:	006517b3          	sll	a5,a0,t1
1c008536:	00c55633          	srl	a2,a0,a2
1c00853a:	006595b3          	sll	a1,a1,t1
1c00853e:	0108d513          	srli	a0,a7,0x10
1c008542:	8dd1                	or	a1,a1,a2
1c008544:	02a77633          	remu	a2,a4,a0
1c008548:	1008d833          	p.exthz	a6,a7
1c00854c:	0105d693          	srli	a3,a1,0x10
1c008550:	02a75733          	divu	a4,a4,a0
1c008554:	0642                	slli	a2,a2,0x10
1c008556:	8ed1                	or	a3,a3,a2
1c008558:	02e80733          	mul	a4,a6,a4
1c00855c:	00e6f863          	bleu	a4,a3,1c00856c <__umoddi3+0x194>
1c008560:	96c6                	add	a3,a3,a7
1c008562:	0116e563          	bltu	a3,a7,1c00856c <__umoddi3+0x194>
1c008566:	00e6f363          	bleu	a4,a3,1c00856c <__umoddi3+0x194>
1c00856a:	96c6                	add	a3,a3,a7
1c00856c:	8e99                	sub	a3,a3,a4
1c00856e:	02a6f733          	remu	a4,a3,a0
1c008572:	02a6d6b3          	divu	a3,a3,a0
1c008576:	df0725b3          	p.insert	a1,a4,15,16
1c00857a:	02d806b3          	mul	a3,a6,a3
1c00857e:	00d5f863          	bleu	a3,a1,1c00858e <__umoddi3+0x1b6>
1c008582:	95c6                	add	a1,a1,a7
1c008584:	0115e563          	bltu	a1,a7,1c00858e <__umoddi3+0x1b6>
1c008588:	00d5f363          	bleu	a3,a1,1c00858e <__umoddi3+0x1b6>
1c00858c:	95c6                	add	a1,a1,a7
1c00858e:	8d95                	sub	a1,a1,a3
1c008590:	bf25                	j	1c0084c8 <__umoddi3+0xf0>
1c008592:	eed5e9e3          	bltu	a1,a3,1c008484 <__umoddi3+0xac>
1c008596:	6741                	lui	a4,0x10
1c008598:	04e6f563          	bleu	a4,a3,1c0085e2 <__umoddi3+0x20a>
1c00859c:	0ff00e93          	li	t4,255
1c0085a0:	00deb733          	sltu	a4,t4,a3
1c0085a4:	070e                	slli	a4,a4,0x3
1c0085a6:	1c001337          	lui	t1,0x1c001
1c0085aa:	00e6d8b3          	srl	a7,a3,a4
1c0085ae:	86030313          	addi	t1,t1,-1952 # 1c000860 <__DTOR_END__>
1c0085b2:	989a                	add	a7,a7,t1
1c0085b4:	0008ce83          	lbu	t4,0(a7)
1c0085b8:	02000e13          	li	t3,32
1c0085bc:	9eba                	add	t4,t4,a4
1c0085be:	41de0e33          	sub	t3,t3,t4
1c0085c2:	020e1763          	bnez	t3,1c0085f0 <__umoddi3+0x218>
1c0085c6:	00b6e463          	bltu	a3,a1,1c0085ce <__umoddi3+0x1f6>
1c0085ca:	00c56963          	bltu	a0,a2,1c0085dc <__umoddi3+0x204>
1c0085ce:	40c507b3          	sub	a5,a0,a2
1c0085d2:	8d95                	sub	a1,a1,a3
1c0085d4:	00f53533          	sltu	a0,a0,a5
1c0085d8:	40a58833          	sub	a6,a1,a0
1c0085dc:	853e                	mv	a0,a5
1c0085de:	85c2                	mv	a1,a6
1c0085e0:	b555                	j	1c008484 <__umoddi3+0xac>
1c0085e2:	010008b7          	lui	a7,0x1000
1c0085e6:	4741                	li	a4,16
1c0085e8:	fb16efe3          	bltu	a3,a7,1c0085a6 <__umoddi3+0x1ce>
1c0085ec:	4761                	li	a4,24
1c0085ee:	bf65                	j	1c0085a6 <__umoddi3+0x1ce>
1c0085f0:	01d65733          	srl	a4,a2,t4
1c0085f4:	01c696b3          	sll	a3,a3,t3
1c0085f8:	8ed9                	or	a3,a3,a4
1c0085fa:	01d5d7b3          	srl	a5,a1,t4
1c0085fe:	0106d813          	srli	a6,a3,0x10
1c008602:	0307f333          	remu	t1,a5,a6
1c008606:	1006d733          	p.exthz	a4,a3
1c00860a:	01d558b3          	srl	a7,a0,t4
1c00860e:	01c595b3          	sll	a1,a1,t3
1c008612:	00b8e5b3          	or	a1,a7,a1
1c008616:	0105d893          	srli	a7,a1,0x10
1c00861a:	01c61633          	sll	a2,a2,t3
1c00861e:	01c51533          	sll	a0,a0,t3
1c008622:	0307d7b3          	divu	a5,a5,a6
1c008626:	0342                	slli	t1,t1,0x10
1c008628:	011368b3          	or	a7,t1,a7
1c00862c:	02f70f33          	mul	t5,a4,a5
1c008630:	833e                	mv	t1,a5
1c008632:	01e8fc63          	bleu	t5,a7,1c00864a <__umoddi3+0x272>
1c008636:	98b6                	add	a7,a7,a3
1c008638:	fff78313          	addi	t1,a5,-1 # ffffff <__l1_heap_size+0xff0003>
1c00863c:	00d8e763          	bltu	a7,a3,1c00864a <__umoddi3+0x272>
1c008640:	01e8f563          	bleu	t5,a7,1c00864a <__umoddi3+0x272>
1c008644:	ffe78313          	addi	t1,a5,-2
1c008648:	98b6                	add	a7,a7,a3
1c00864a:	41e888b3          	sub	a7,a7,t5
1c00864e:	0308f7b3          	remu	a5,a7,a6
1c008652:	0308d8b3          	divu	a7,a7,a6
1c008656:	df07a5b3          	p.insert	a1,a5,15,16
1c00865a:	03170733          	mul	a4,a4,a7
1c00865e:	87c6                	mv	a5,a7
1c008660:	00e5fc63          	bleu	a4,a1,1c008678 <__umoddi3+0x2a0>
1c008664:	95b6                	add	a1,a1,a3
1c008666:	fff88793          	addi	a5,a7,-1 # ffffff <__l1_heap_size+0xff0003>
1c00866a:	00d5e763          	bltu	a1,a3,1c008678 <__umoddi3+0x2a0>
1c00866e:	00e5f563          	bleu	a4,a1,1c008678 <__umoddi3+0x2a0>
1c008672:	ffe88793          	addi	a5,a7,-2
1c008676:	95b6                	add	a1,a1,a3
1c008678:	0342                	slli	t1,t1,0x10
1c00867a:	6f41                	lui	t5,0x10
1c00867c:	00f36333          	or	t1,t1,a5
1c008680:	ffff0793          	addi	a5,t5,-1 # ffff <__l1_heap_size+0x3>
1c008684:	00f37833          	and	a6,t1,a5
1c008688:	01035313          	srli	t1,t1,0x10
1c00868c:	8ff1                	and	a5,a5,a2
1c00868e:	02f808b3          	mul	a7,a6,a5
1c008692:	8d99                	sub	a1,a1,a4
1c008694:	01065713          	srli	a4,a2,0x10
1c008698:	02f307b3          	mul	a5,t1,a5
1c00869c:	8fbe                	mv	t6,a5
1c00869e:	42e80fb3          	p.mac	t6,a6,a4
1c0086a2:	0108d813          	srli	a6,a7,0x10
1c0086a6:	987e                	add	a6,a6,t6
1c0086a8:	02e30333          	mul	t1,t1,a4
1c0086ac:	00f87363          	bleu	a5,a6,1c0086b2 <__umoddi3+0x2da>
1c0086b0:	937a                	add	t1,t1,t5
1c0086b2:	01085713          	srli	a4,a6,0x10
1c0086b6:	933a                	add	t1,t1,a4
1c0086b8:	6741                	lui	a4,0x10
1c0086ba:	177d                	addi	a4,a4,-1
1c0086bc:	00e87833          	and	a6,a6,a4
1c0086c0:	0842                	slli	a6,a6,0x10
1c0086c2:	00e8f733          	and	a4,a7,a4
1c0086c6:	9742                	add	a4,a4,a6
1c0086c8:	0065e663          	bltu	a1,t1,1c0086d4 <__umoddi3+0x2fc>
1c0086cc:	00659d63          	bne	a1,t1,1c0086e6 <__umoddi3+0x30e>
1c0086d0:	00e57b63          	bleu	a4,a0,1c0086e6 <__umoddi3+0x30e>
1c0086d4:	40c70633          	sub	a2,a4,a2
1c0086d8:	00c73733          	sltu	a4,a4,a2
1c0086dc:	40d30333          	sub	t1,t1,a3
1c0086e0:	40e30333          	sub	t1,t1,a4
1c0086e4:	8732                	mv	a4,a2
1c0086e6:	40e50733          	sub	a4,a0,a4
1c0086ea:	00e53533          	sltu	a0,a0,a4
1c0086ee:	406585b3          	sub	a1,a1,t1
1c0086f2:	8d89                	sub	a1,a1,a0
1c0086f4:	01d597b3          	sll	a5,a1,t4
1c0086f8:	01c75733          	srl	a4,a4,t3
1c0086fc:	00e7e533          	or	a0,a5,a4
1c008700:	01c5d5b3          	srl	a1,a1,t3
1c008704:	b341                	j	1c008484 <__umoddi3+0xac>

1c008706 <__extendsfdf2>:
1c008706:	002027f3          	frrm	a5
1c00870a:	cf751633          	p.extractu	a2,a0,7,23
1c00870e:	00160713          	addi	a4,a2,1 # 1000001 <__l1_heap_size+0xff0005>
1c008712:	ee873733          	p.bclr	a4,a4,23,8
1c008716:	4685                	li	a3,1
1c008718:	ec0517b3          	p.extractu	a5,a0,22,0
1c00871c:	817d                	srli	a0,a0,0x1f
1c00871e:	00e6d963          	ble	a4,a3,1c008730 <__extendsfdf2+0x2a>
1c008722:	0037d713          	srli	a4,a5,0x3
1c008726:	38060613          	addi	a2,a2,896
1c00872a:	07f6                	slli	a5,a5,0x1d
1c00872c:	4681                	li	a3,0
1c00872e:	a899                	j	1c008784 <__extendsfdf2+0x7e>
1c008730:	ee05                	bnez	a2,1c008768 <__extendsfdf2+0x62>
1c008732:	c7ad                	beqz	a5,1c00879c <__extendsfdf2+0x96>
1c008734:	477d                	li	a4,31
1c008736:	100796b3          	p.fl1	a3,a5
1c00873a:	40d706b3          	sub	a3,a4,a3
1c00873e:	4729                	li	a4,10
1c008740:	00d74e63          	blt	a4,a3,1c00875c <__extendsfdf2+0x56>
1c008744:	472d                	li	a4,11
1c008746:	8f15                	sub	a4,a4,a3
1c008748:	01568613          	addi	a2,a3,21
1c00874c:	00e7d733          	srl	a4,a5,a4
1c008750:	00c797b3          	sll	a5,a5,a2
1c008754:	38900613          	li	a2,905
1c008758:	8e15                	sub	a2,a2,a3
1c00875a:	bfc9                	j	1c00872c <__extendsfdf2+0x26>
1c00875c:	ff568713          	addi	a4,a3,-11
1c008760:	00e79733          	sll	a4,a5,a4
1c008764:	4781                	li	a5,0
1c008766:	b7fd                	j	1c008754 <__extendsfdf2+0x4e>
1c008768:	cf8d                	beqz	a5,1c0087a2 <__extendsfdf2+0x9c>
1c00876a:	004006b7          	lui	a3,0x400
1c00876e:	8efd                	and	a3,a3,a5
1c008770:	0037d713          	srli	a4,a5,0x3
1c008774:	0016b693          	seqz	a3,a3
1c008778:	0692                	slli	a3,a3,0x4
1c00877a:	c1374733          	p.bset	a4,a4,0,19
1c00877e:	07f6                	slli	a5,a5,0x1d
1c008780:	7ff00613          	li	a2,2047
1c008784:	4581                	li	a1,0
1c008786:	e60725b3          	p.insert	a1,a4,19,0
1c00878a:	d54625b3          	p.insert	a1,a2,10,20
1c00878e:	c1f525b3          	p.insert	a1,a0,0,31
1c008792:	853e                	mv	a0,a5
1c008794:	c299                	beqz	a3,1c00879a <__extendsfdf2+0x94>
1c008796:	0016a073          	csrs	0x1,a3
1c00879a:	8082                	ret
1c00879c:	4701                	li	a4,0
1c00879e:	4601                	li	a2,0
1c0087a0:	b771                	j	1c00872c <__extendsfdf2+0x26>
1c0087a2:	4701                	li	a4,0
1c0087a4:	7ff00613          	li	a2,2047
1c0087a8:	b751                	j	1c00872c <__extendsfdf2+0x26>

1c0087aa <main>:
1c0087aa:	1141                	addi	sp,sp,-16
1c0087ac:	1c0016b7          	lui	a3,0x1c001
1c0087b0:	1c001637          	lui	a2,0x1c001
1c0087b4:	24068813          	addi	a6,a3,576 # 1c001240 <_edata>
1c0087b8:	44060313          	addi	t1,a2,1088 # 1c001440 <B>
1c0087bc:	c606                	sw	ra,12(sp)
1c0087be:	c422                	sw	s0,8(sp)
1c0087c0:	c226                	sw	s1,4(sp)
1c0087c2:	44060613          	addi	a2,a2,1088
1c0087c6:	24068693          	addi	a3,a3,576
1c0087ca:	4781                	li	a5,0
1c0087cc:	080350fb          	lp.setupi	x1,128,1c0087d8 <main+0x2e>
1c0087d0:	00f6a22b          	p.sw	a5,4(a3!)
1c0087d4:	00f6222b          	p.sw	a5,4(a2!)
1c0087d8:	0785                	addi	a5,a5,1
1c0087da:	000207b7          	lui	a5,0x20
1c0087de:	0789                	addi	a5,a5,2
1c0087e0:	cc079073          	csrw	0xcc0,a5
1c0087e4:	f1402e73          	csrr	t3,mhartid
1c0087e8:	47fd                	li	a5,31
1c0087ea:	ca5e1733          	p.extractu	a4,t3,5,5
1c0087ee:	08f70c63          	beq	a4,a5,1c008886 <main+0xdc>
1c0087f2:	ca5e17b3          	p.extractu	a5,t3,5,5
1c0087f6:	477d                	li	a4,31
1c0087f8:	06e78e63          	beq	a5,a4,1c008874 <main+0xca>
1c0087fc:	1c0018b7          	lui	a7,0x1c001
1c008800:	64088893          	addi	a7,a7,1600 # 1c001640 <tempC>
1c008804:	0807507b          	lp.setupi	x0,128,1c008820 <main+0x76>
1c008808:	0048258b          	p.lw	a1,4(a6!)
1c00880c:	8646                	mv	a2,a7
1c00880e:	869a                	mv	a3,t1
1c008810:	080350fb          	lp.setupi	x1,128,1c00881c <main+0x72>
1c008814:	0046a78b          	p.lw	a5,4(a3!)
1c008818:	02f587b3          	mul	a5,a1,a5
1c00881c:	00f6222b          	p.sw	a5,4(a2!)
1c008820:	0001                	nop
1c008822:	ca5e17b3          	p.extractu	a5,t3,5,5
1c008826:	477d                	li	a4,31
1c008828:	08e78263          	beq	a5,a4,1c0088ac <main+0x102>
1c00882c:	ca5e17b3          	p.extractu	a5,t3,5,5
1c008830:	477d                	li	a4,31
1c008832:	4401                	li	s0,0
1c008834:	06e78963          	beq	a5,a4,1c0088a6 <main+0xfc>
1c008838:	ca5e1e33          	p.extractu	t3,t3,5,5
1c00883c:	47fd                	li	a5,31
1c00883e:	f00007d3          	fmv.s.x	fa5,zero
1c008842:	4481                	li	s1,0
1c008844:	04fe0a63          	beq	t3,a5,1c008898 <main+0xee>
1c008848:	d0147753          	fcvt.s.wu	fa4,s0
1c00884c:	18e7f753          	fdiv.s	fa4,fa5,fa4
1c008850:	e0070553          	fmv.x.s	a0,fa4
1c008854:	3d4d                	jal	1c008706 <__extendsfdf2>
1c008856:	872a                	mv	a4,a0
1c008858:	1c001537          	lui	a0,0x1c001
1c00885c:	87ae                	mv	a5,a1
1c00885e:	8626                	mv	a2,s1
1c008860:	85a2                	mv	a1,s0
1c008862:	96050513          	addi	a0,a0,-1696 # 1c000960 <__DTOR_END__+0x100>
1c008866:	22dd                	jal	1c008a4c <printf>
1c008868:	40b2                	lw	ra,12(sp)
1c00886a:	4422                	lw	s0,8(sp)
1c00886c:	4492                	lw	s1,4(sp)
1c00886e:	4501                	li	a0,0
1c008870:	0141                	addi	sp,sp,16
1c008872:	8082                	ret
1c008874:	4785                	li	a5,1
1c008876:	1a10b737          	lui	a4,0x1a10b
1c00887a:	00f72c23          	sw	a5,24(a4) # 1a10b018 <__l1_end+0xa10b014>
1c00887e:	478d                	li	a5,3
1c008880:	cc179073          	csrw	0xcc1,a5
1c008884:	bfa5                	j	1c0087fc <main+0x52>
1c008886:	4785                	li	a5,1
1c008888:	1a10b737          	lui	a4,0x1a10b
1c00888c:	02f72023          	sw	a5,32(a4) # 1a10b020 <__l1_end+0xa10b01c>
1c008890:	4781                	li	a5,0
1c008892:	79f79073          	csrw	pccr31,a5
1c008896:	bfb1                	j	1c0087f2 <main+0x48>
1c008898:	1a10b637          	lui	a2,0x1a10b
1c00889c:	00862483          	lw	s1,8(a2) # 1a10b008 <__l1_end+0xa10b004>
1c0088a0:	d014f7d3          	fcvt.s.wu	fa5,s1
1c0088a4:	b755                	j	1c008848 <main+0x9e>
1c0088a6:	78102473          	csrr	s0,pccr1
1c0088aa:	b779                	j	1c008838 <main+0x8e>
1c0088ac:	1a10b7b7          	lui	a5,0x1a10b
1c0088b0:	0007a023          	sw	zero,0(a5) # 1a10b000 <__l1_end+0xa10affc>
1c0088b4:	4781                	li	a5,0
1c0088b6:	cc179073          	csrw	0xcc1,a5
1c0088ba:	bf8d                	j	1c00882c <main+0x82>

1c0088bc <pos_fll_init>:
1c0088bc:	00451593          	slli	a1,a0,0x4
1c0088c0:	832a                	mv	t1,a0
1c0088c2:	00458e13          	addi	t3,a1,4
1c0088c6:	1a100637          	lui	a2,0x1a100
1c0088ca:	21c67703          	p.lw	a4,t3(a2)
1c0088ce:	86ba                	mv	a3,a4
1c0088d0:	04074163          	bltz	a4,1c008912 <pos_fll_init+0x56>
1c0088d4:	00858793          	addi	a5,a1,8
1c0088d8:	20f67883          	p.lw	a7,a5(a2)
1c0088dc:	4519                	li	a0,6
1c0088de:	caa528b3          	p.insert	a7,a0,5,10
1c0088e2:	05000513          	li	a0,80
1c0088e6:	d70528b3          	p.insert	a7,a0,11,16
1c0088ea:	1a100837          	lui	a6,0x1a100
1c0088ee:	011867a3          	p.sw	a7,a5(a6)
1c0088f2:	05b1                	addi	a1,a1,12
1c0088f4:	20b67603          	p.lw	a2,a1(a2)
1c0088f8:	14c00793          	li	a5,332
1c0088fc:	d307a633          	p.insert	a2,a5,9,16
1c008900:	00c865a3          	p.sw	a2,a1(a6)
1c008904:	4785                	li	a5,1
1c008906:	c1e7a6b3          	p.insert	a3,a5,0,30
1c00890a:	c1f7a6b3          	p.insert	a3,a5,0,31
1c00890e:	00d86e23          	p.sw	a3,t3(a6)
1c008912:	00400793          	li	a5,4
1c008916:	00231613          	slli	a2,t1,0x2
1c00891a:	963e                	add	a2,a2,a5
1c00891c:	4208                	lw	a0,0(a2)
1c00891e:	ed19                	bnez	a0,1c00893c <pos_fll_init+0x80>
1c008920:	10075733          	p.exthz	a4,a4
1c008924:	c7a696b3          	p.extractu	a3,a3,3,26
1c008928:	073e                	slli	a4,a4,0xf
1c00892a:	16fd                	addi	a3,a3,-1
1c00892c:	00d75533          	srl	a0,a4,a3
1c008930:	c208                	sw	a0,0(a2)
1c008932:	979a                	add	a5,a5,t1
1c008934:	4705                	li	a4,1
1c008936:	00e78423          	sb	a4,8(a5)
1c00893a:	8082                	ret
1c00893c:	46f5                	li	a3,29
1c00893e:	10051733          	p.fl1	a4,a0
1c008942:	4585                	li	a1,1
1c008944:	82e6b75b          	p.subun	a4,a3,a4,1
1c008948:	04e5e733          	p.max	a4,a1,a4
1c00894c:	fff70893          	addi	a7,a4,-1
1c008950:	00e51833          	sll	a6,a0,a4
1c008954:	006786b3          	add	a3,a5,t1
1c008958:	011595b3          	sll	a1,a1,a7
1c00895c:	0086c683          	lbu	a3,8(a3)
1c008960:	dc0838b3          	p.bclr	a7,a6,14,0
1c008964:	c0e8a5db          	p.addunr	a1,a7,a4
1c008968:	c20c                	sw	a1,0(a2)
1c00896a:	d6e1                	beqz	a3,1c008932 <pos_fll_init+0x76>
1c00896c:	1a100637          	lui	a2,0x1a100
1c008970:	21c67683          	p.lw	a3,t3(a2)
1c008974:	00f85813          	srli	a6,a6,0xf
1c008978:	de0826b3          	p.insert	a3,a6,15,0
1c00897c:	0705                	addi	a4,a4,1
1c00897e:	c7a726b3          	p.insert	a3,a4,3,26
1c008982:	00d66e23          	p.sw	a3,t3(a2)
1c008986:	979a                	add	a5,a5,t1
1c008988:	4705                	li	a4,1
1c00898a:	00e78423          	sb	a4,8(a5)
1c00898e:	8082                	ret

1c008990 <pos_fll_constructor>:
1c008990:	00400793          	li	a5,4
1c008994:	0007a023          	sw	zero,0(a5)
1c008998:	0007a223          	sw	zero,4(a5)
1c00899c:	00079423          	sh	zero,8(a5)
1c0089a0:	8082                	ret

1c0089a2 <pos_soc_init>:
1c0089a2:	1141                	addi	sp,sp,-16
1c0089a4:	c606                	sw	ra,12(sp)
1c0089a6:	c422                	sw	s0,8(sp)
1c0089a8:	37e5                	jal	1c008990 <pos_fll_constructor>
1c0089aa:	4501                	li	a0,0
1c0089ac:	3f01                	jal	1c0088bc <pos_fll_init>
1c0089ae:	01000413          	li	s0,16
1c0089b2:	c008                	sw	a0,0(s0)
1c0089b4:	4505                	li	a0,1
1c0089b6:	3719                	jal	1c0088bc <pos_fll_init>
1c0089b8:	c408                	sw	a0,8(s0)
1c0089ba:	40b2                	lw	ra,12(sp)
1c0089bc:	4422                	lw	s0,8(sp)
1c0089be:	0141                	addi	sp,sp,16
1c0089c0:	8082                	ret

1c0089c2 <strchr>:
1c0089c2:	00054783          	lbu	a5,0(a0)
1c0089c6:	0ff5f593          	andi	a1,a1,255
1c0089ca:	00b78c63          	beq	a5,a1,1c0089e2 <strchr+0x20>
1c0089ce:	cb99                	beqz	a5,1c0089e4 <strchr+0x22>
1c0089d0:	00150793          	addi	a5,a0,1
1c0089d4:	a011                	j	1c0089d8 <strchr+0x16>
1c0089d6:	c719                	beqz	a4,1c0089e4 <strchr+0x22>
1c0089d8:	853e                	mv	a0,a5
1c0089da:	0017c70b          	p.lbu	a4,1(a5!)
1c0089de:	feb71ce3          	bne	a4,a1,1c0089d6 <strchr+0x14>
1c0089e2:	8082                	ret
1c0089e4:	0015b593          	seqz	a1,a1
1c0089e8:	40b005b3          	neg	a1,a1
1c0089ec:	8d6d                	and	a0,a0,a1
1c0089ee:	8082                	ret

1c0089f0 <pos_libc_fputc_locked>:
1c0089f0:	f14027f3          	csrr	a5,mhartid
1c0089f4:	1a10f6b7          	lui	a3,0x1a10f
1c0089f8:	f1402773          	csrr	a4,mhartid
1c0089fc:	ca5797b3          	p.extractu	a5,a5,5,5
1c008a00:	070e                	slli	a4,a4,0x3
1c008a02:	079e                	slli	a5,a5,0x7
1c008a04:	0ff57513          	andi	a0,a0,255
1c008a08:	ee873733          	p.bclr	a4,a4,23,8
1c008a0c:	97b6                	add	a5,a5,a3
1c008a0e:	00a767a3          	p.sw	a0,a5(a4)
1c008a12:	4501                	li	a0,0
1c008a14:	8082                	ret

1c008a16 <pos_libc_prf_locked>:
1c008a16:	a095                	j	1c008a7a <pos_libc_prf>

1c008a18 <exit>:
1c008a18:	1141                	addi	sp,sp,-16
1c008a1a:	c422                	sw	s0,8(sp)
1c008a1c:	c606                	sw	ra,12(sp)
1c008a1e:	842a                	mv	s0,a0
1c008a20:	461010ef          	jal	ra,1c00a680 <pos_init_stop>
1c008a24:	c1f44533          	p.bset	a0,s0,0,31
1c008a28:	1a1047b7          	lui	a5,0x1a104
1c008a2c:	0aa7a023          	sw	a0,160(a5) # 1a1040a0 <__l1_end+0xa10409c>
1c008a30:	1a10a7b7          	lui	a5,0x1a10a
1c008a34:	577d                	li	a4,-1
1c008a36:	80e7a423          	sw	a4,-2040(a5) # 1a109808 <__l1_end+0xa109804>
1c008a3a:	10500073          	wfi
1c008a3e:	10500073          	wfi
1c008a42:	bfe5                	j	1c008a3a <exit+0x22>

1c008a44 <pos_io_start>:
1c008a44:	4501                	li	a0,0
1c008a46:	8082                	ret

1c008a48 <pos_io_stop>:
1c008a48:	4501                	li	a0,0
1c008a4a:	8082                	ret

1c008a4c <printf>:
1c008a4c:	7139                	addi	sp,sp,-64
1c008a4e:	02410313          	addi	t1,sp,36
1c008a52:	d432                	sw	a2,40(sp)
1c008a54:	862a                	mv	a2,a0
1c008a56:	1c009537          	lui	a0,0x1c009
1c008a5a:	d22e                	sw	a1,36(sp)
1c008a5c:	d636                	sw	a3,44(sp)
1c008a5e:	4589                	li	a1,2
1c008a60:	869a                	mv	a3,t1
1c008a62:	9f050513          	addi	a0,a0,-1552 # 1c0089f0 <pos_libc_fputc_locked>
1c008a66:	ce06                	sw	ra,28(sp)
1c008a68:	d83a                	sw	a4,48(sp)
1c008a6a:	da3e                	sw	a5,52(sp)
1c008a6c:	dc42                	sw	a6,56(sp)
1c008a6e:	de46                	sw	a7,60(sp)
1c008a70:	c61a                	sw	t1,12(sp)
1c008a72:	3755                	jal	1c008a16 <pos_libc_prf_locked>
1c008a74:	40f2                	lw	ra,28(sp)
1c008a76:	6121                	addi	sp,sp,64
1c008a78:	8082                	ret

1c008a7a <pos_libc_prf>:
1c008a7a:	7119                	addi	sp,sp,-128
1c008a7c:	105c                	addi	a5,sp,36
1c008a7e:	dca2                	sw	s0,120(sp)
1c008a80:	daa6                	sw	s1,116(sp)
1c008a82:	d8ca                	sw	s2,112(sp)
1c008a84:	d4d2                	sw	s4,104(sp)
1c008a86:	d2d6                	sw	s5,100(sp)
1c008a88:	cae6                	sw	s9,84(sp)
1c008a8a:	de86                	sw	ra,124(sp)
1c008a8c:	d6ce                	sw	s3,108(sp)
1c008a8e:	d0da                	sw	s6,96(sp)
1c008a90:	cede                	sw	s7,92(sp)
1c008a92:	cce2                	sw	s8,88(sp)
1c008a94:	c8ea                	sw	s10,80(sp)
1c008a96:	c6ee                	sw	s11,76(sp)
1c008a98:	892a                	mv	s2,a0
1c008a9a:	84ae                	mv	s1,a1
1c008a9c:	8cb2                	mv	s9,a2
1c008a9e:	8ab6                	mv	s5,a3
1c008aa0:	4401                	li	s0,0
1c008aa2:	1c001a37          	lui	s4,0x1c001
1c008aa6:	c03e                	sw	a5,0(sp)
1c008aa8:	a039                	j	1c008ab6 <pos_libc_prf+0x3c>
1c008aaa:	85a6                	mv	a1,s1
1c008aac:	9902                	jalr	s2
1c008aae:	09f52c63          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008ab2:	0405                	addi	s0,s0,1
1c008ab4:	8cea                	mv	s9,s10
1c008ab6:	000cc503          	lbu	a0,0(s9)
1c008aba:	001c8d13          	addi	s10,s9,1
1c008abe:	c549                	beqz	a0,1c008b48 <pos_libc_prf+0xce>
1c008ac0:	02500793          	li	a5,37
1c008ac4:	fef513e3          	bne	a0,a5,1c008aaa <pos_libc_prf+0x30>
1c008ac8:	ca02                	sw	zero,20(sp)
1c008aca:	c802                	sw	zero,16(sp)
1c008acc:	c602                	sw	zero,12(sp)
1c008ace:	c402                	sw	zero,8(sp)
1c008ad0:	c202                	sw	zero,4(sp)
1c008ad2:	02300993          	li	s3,35
1c008ad6:	02d00b13          	li	s6,45
1c008ada:	000d4d83          	lbu	s11,0(s10)
1c008ade:	001d0b93          	addi	s7,s10,1
1c008ae2:	998a0513          	addi	a0,s4,-1640 # 1c000998 <__DTOR_END__+0x138>
1c008ae6:	85ee                	mv	a1,s11
1c008ae8:	8cde                	mv	s9,s7
1c008aea:	8c6e                	mv	s8,s11
1c008aec:	3dd9                	jal	1c0089c2 <strchr>
1c008aee:	c55d                	beqz	a0,1c008b9c <pos_libc_prf+0x122>
1c008af0:	0b3d8263          	beq	s11,s3,1c008b94 <pos_libc_prf+0x11a>
1c008af4:	07b9fa63          	bleu	s11,s3,1c008b68 <pos_libc_prf+0xee>
1c008af8:	096d8a63          	beq	s11,s6,1c008b8c <pos_libc_prf+0x112>
1c008afc:	03000793          	li	a5,48
1c008b00:	08fd8263          	beq	s11,a5,1c008b84 <pos_libc_prf+0x10a>
1c008b04:	02b00793          	li	a5,43
1c008b08:	06fd8a63          	beq	s11,a5,1c008b7c <pos_libc_prf+0x102>
1c008b0c:	8d5e                	mv	s10,s7
1c008b0e:	b7f1                	j	1c008ada <pos_libc_prf+0x60>
1c008b10:	000c4503          	lbu	a0,0(s8)
1c008b14:	46a5                	li	a3,9
1c008b16:	fd050613          	addi	a2,a0,-48
1c008b1a:	00c6f463          	bleu	a2,a3,1c008b22 <pos_libc_prf+0xa8>
1c008b1e:	0420106f          	j	1c009b60 <pos_libc_prf+0x10e6>
1c008b22:	001c0b93          	addi	s7,s8,1
1c008b26:	a819                	j	1c008b3c <pos_libc_prf+0xc2>
1c008b28:	001bc50b          	p.lbu	a0,1(s7!)
1c008b2c:	47a5                	li	a5,9
1c008b2e:	1d7d                	addi	s10,s10,-1
1c008b30:	fd050613          	addi	a2,a0,-48
1c008b34:	00c7f463          	bleu	a2,a5,1c008b3c <pos_libc_prf+0xc2>
1c008b38:	02a0106f          	j	1c009b62 <pos_libc_prf+0x10e8>
1c008b3c:	85a6                	mv	a1,s1
1c008b3e:	8c5e                	mv	s8,s7
1c008b40:	9902                	jalr	s2
1c008b42:	fff533e3          	p.bneimm	a0,-1,1c008b28 <pos_libc_prf+0xae>
1c008b46:	547d                	li	s0,-1
1c008b48:	8522                	mv	a0,s0
1c008b4a:	50f6                	lw	ra,124(sp)
1c008b4c:	5466                	lw	s0,120(sp)
1c008b4e:	54d6                	lw	s1,116(sp)
1c008b50:	5946                	lw	s2,112(sp)
1c008b52:	59b6                	lw	s3,108(sp)
1c008b54:	5a26                	lw	s4,104(sp)
1c008b56:	5a96                	lw	s5,100(sp)
1c008b58:	5b06                	lw	s6,96(sp)
1c008b5a:	4bf6                	lw	s7,92(sp)
1c008b5c:	4c66                	lw	s8,88(sp)
1c008b5e:	4cd6                	lw	s9,84(sp)
1c008b60:	4d46                	lw	s10,80(sp)
1c008b62:	4db6                	lw	s11,76(sp)
1c008b64:	6109                	addi	sp,sp,128
1c008b66:	8082                	ret
1c008b68:	fe0d80e3          	beqz	s11,1c008b48 <pos_libc_prf+0xce>
1c008b6c:	02000793          	li	a5,32
1c008b70:	f8fd9ee3          	bne	s11,a5,1c008b0c <pos_libc_prf+0x92>
1c008b74:	4785                	li	a5,1
1c008b76:	c83e                	sw	a5,16(sp)
1c008b78:	8d5e                	mv	s10,s7
1c008b7a:	b785                	j	1c008ada <pos_libc_prf+0x60>
1c008b7c:	4785                	li	a5,1
1c008b7e:	c63e                	sw	a5,12(sp)
1c008b80:	8d5e                	mv	s10,s7
1c008b82:	bfa1                	j	1c008ada <pos_libc_prf+0x60>
1c008b84:	4785                	li	a5,1
1c008b86:	ca3e                	sw	a5,20(sp)
1c008b88:	8d5e                	mv	s10,s7
1c008b8a:	bf81                	j	1c008ada <pos_libc_prf+0x60>
1c008b8c:	4785                	li	a5,1
1c008b8e:	c43e                	sw	a5,8(sp)
1c008b90:	8d5e                	mv	s10,s7
1c008b92:	b7a1                	j	1c008ada <pos_libc_prf+0x60>
1c008b94:	4785                	li	a5,1
1c008b96:	c23e                	sw	a5,4(sp)
1c008b98:	8d5e                	mv	s10,s7
1c008b9a:	b781                	j	1c008ada <pos_libc_prf+0x60>
1c008b9c:	02a00593          	li	a1,42
1c008ba0:	26bd8663          	beq	s11,a1,1c008e0c <pos_libc_prf+0x392>
1c008ba4:	fd0d8593          	addi	a1,s11,-48
1c008ba8:	4525                	li	a0,9
1c008baa:	4b01                	li	s6,0
1c008bac:	30b57463          	bleu	a1,a0,1c008eb4 <pos_libc_prf+0x43a>
1c008bb0:	02e00713          	li	a4,46
1c008bb4:	20ec0a63          	beq	s8,a4,1c008dc8 <pos_libc_prf+0x34e>
1c008bb8:	8be2                	mv	s7,s8
1c008bba:	59fd                	li	s3,-1
1c008bbc:	1c0017b7          	lui	a5,0x1c001
1c008bc0:	85e2                	mv	a1,s8
1c008bc2:	9a078513          	addi	a0,a5,-1632 # 1c0009a0 <__DTOR_END__+0x140>
1c008bc6:	3bf5                	jal	1c0089c2 <strchr>
1c008bc8:	4701                	li	a4,0
1c008bca:	cd19                	beqz	a0,1c008be8 <pos_libc_prf+0x16e>
1c008bcc:	06c00713          	li	a4,108
1c008bd0:	001c8693          	addi	a3,s9,1
1c008bd4:	000ccc03          	lbu	s8,0(s9)
1c008bd8:	2ceb8a63          	beq	s7,a4,1c008eac <pos_libc_prf+0x432>
1c008bdc:	06800713          	li	a4,104
1c008be0:	42eb8763          	beq	s7,a4,1c00900e <pos_libc_prf+0x594>
1c008be4:	8cb6                	mv	s9,a3
1c008be6:	875e                	mv	a4,s7
1c008be8:	06700693          	li	a3,103
1c008bec:	0386ec63          	bltu	a3,s8,1c008c24 <pos_libc_prf+0x1aa>
1c008bf0:	06500693          	li	a3,101
1c008bf4:	22dc7ee3          	bleu	a3,s8,1c009630 <pos_libc_prf+0xbb6>
1c008bf8:	04700693          	li	a3,71
1c008bfc:	2f86e163          	bltu	a3,s8,1c008ede <pos_libc_prf+0x464>
1c008c00:	04500713          	li	a4,69
1c008c04:	22ec76e3          	bleu	a4,s8,1c009630 <pos_libc_prf+0xbb6>
1c008c08:	f40c00e3          	beqz	s8,1c008b48 <pos_libc_prf+0xce>
1c008c0c:	02500713          	li	a4,37
1c008c10:	44ec15e3          	bne	s8,a4,1c00985a <pos_libc_prf+0xde0>
1c008c14:	85a6                	mv	a1,s1
1c008c16:	02500513          	li	a0,37
1c008c1a:	9902                	jalr	s2
1c008c1c:	f3f525e3          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008c20:	0405                	addi	s0,s0,1
1c008c22:	bd51                	j	1c008ab6 <pos_libc_prf+0x3c>
1c008c24:	07000693          	li	a3,112
1c008c28:	3adc05e3          	beq	s8,a3,1c0097d2 <pos_libc_prf+0xd58>
1c008c2c:	1f86ea63          	bltu	a3,s8,1c008e20 <pos_libc_prf+0x3a6>
1c008c30:	06e00693          	li	a3,110
1c008c34:	36dc0be3          	beq	s8,a3,1c0097aa <pos_libc_prf+0xd30>
1c008c38:	2b86ed63          	bltu	a3,s8,1c008ef2 <pos_libc_prf+0x478>
1c008c3c:	06900693          	li	a3,105
1c008c40:	40dc1de3          	bne	s8,a3,1c00985a <pos_libc_prf+0xde0>
1c008c44:	06c00693          	li	a3,108
1c008c48:	00d70a63          	beq	a4,a3,1c008c5c <pos_libc_prf+0x1e2>
1c008c4c:	07a00693          	li	a3,122
1c008c50:	00d70663          	beq	a4,a3,1c008c5c <pos_libc_prf+0x1e2>
1c008c54:	04c00693          	li	a3,76
1c008c58:	60d703e3          	beq	a4,a3,1c009a5e <pos_libc_prf+0xfe4>
1c008c5c:	000aa703          	lw	a4,0(s5)
1c008c60:	0a91                	addi	s5,s5,4
1c008c62:	ce56                	sw	s5,28(sp)
1c008c64:	41f75a93          	srai	s5,a4,0x1f
1c008c68:	4c0ac6e3          	bltz	s5,1c009934 <pos_libc_prf+0xeba>
1c008c6c:	47b2                	lw	a5,12(sp)
1c008c6e:	02b00693          	li	a3,43
1c008c72:	e791                	bnez	a5,1c008c7e <pos_libc_prf+0x204>
1c008c74:	47c2                	lw	a5,16(sp)
1c008c76:	680787e3          	beqz	a5,1c009b04 <pos_libc_prf+0x108a>
1c008c7a:	02000693          	li	a3,32
1c008c7e:	02510793          	addi	a5,sp,37
1c008c82:	02d10223          	sb	a3,36(sp)
1c008c86:	8dd6                	mv	s11,s5
1c008c88:	c23e                	sw	a5,4(sp)
1c008c8a:	8c3e                	mv	s8,a5
1c008c8c:	8d3a                	mv	s10,a4
1c008c8e:	a011                	j	1c008c92 <pos_libc_prf+0x218>
1c008c90:	8c36                	mv	s8,a3
1c008c92:	4629                	li	a2,10
1c008c94:	4681                	li	a3,0
1c008c96:	856a                	mv	a0,s10
1c008c98:	85ee                	mv	a1,s11
1c008c9a:	f3eff0ef          	jal	ra,1c0083d8 <__umoddi3>
1c008c9e:	8baa                	mv	s7,a0
1c008ca0:	85ee                	mv	a1,s11
1c008ca2:	856a                	mv	a0,s10
1c008ca4:	4629                	li	a2,10
1c008ca6:	4681                	li	a3,0
1c008ca8:	bf4ff0ef          	jal	ra,1c00809c <__udivdi3>
1c008cac:	030b8813          	addi	a6,s7,48
1c008cb0:	010c0023          	sb	a6,0(s8)
1c008cb4:	00a5e633          	or	a2,a1,a0
1c008cb8:	001c0693          	addi	a3,s8,1
1c008cbc:	8d2a                	mv	s10,a0
1c008cbe:	8dae                	mv	s11,a1
1c008cc0:	fa61                	bnez	a2,1c008c90 <pos_libc_prf+0x216>
1c008cc2:	4792                	lw	a5,4(sp)
1c008cc4:	00068023          	sb	zero,0(a3) # 1a10f000 <__l1_end+0xa10effc>
1c008cc8:	0187fe63          	bleu	s8,a5,1c008ce4 <pos_libc_prf+0x26a>
1c008ccc:	8762                	mv	a4,s8
1c008cce:	8bbe                	mv	s7,a5
1c008cd0:	000bc583          	lbu	a1,0(s7)
1c008cd4:	00074603          	lbu	a2,0(a4)
1c008cd8:	feb70fab          	p.sb	a1,-1(a4!)
1c008cdc:	00cb80ab          	p.sb	a2,1(s7!)
1c008ce0:	feebe8e3          	bltu	s7,a4,1c008cd0 <pos_libc_prf+0x256>
1c008ce4:	4782                	lw	a5,0(sp)
1c008ce6:	40f68bb3          	sub	s7,a3,a5
1c008cea:	47b2                	lw	a5,12(sp)
1c008cec:	3a0798e3          	bnez	a5,1c00989c <pos_libc_prf+0xe22>
1c008cf0:	47c2                	lw	a5,16(sp)
1c008cf2:	3a0795e3          	bnez	a5,1c00989c <pos_libc_prf+0xe22>
1c008cf6:	01fada93          	srli	s5,s5,0x1f
1c008cfa:	c256                	sw	s5,4(sp)
1c008cfc:	3009cf63          	bltz	s3,1c00901a <pos_libc_prf+0x5a0>
1c008d00:	4792                	lw	a5,4(sp)
1c008d02:	417989b3          	sub	s3,s3,s7
1c008d06:	02410c13          	addi	s8,sp,36
1c008d0a:	99be                	add	s3,s3,a5
1c008d0c:	0409e9b3          	p.max	s3,s3,zero
1c008d10:	013b8733          	add	a4,s7,s3
1c008d14:	40eb0b33          	sub	s6,s6,a4
1c008d18:	4d81                	li	s11,0
1c008d1a:	cc02                	sw	zero,24(sp)
1c008d1c:	4a81                	li	s5,0
1c008d1e:	47a2                	lw	a5,8(sp)
1c008d20:	e385                	bnez	a5,1c008d40 <pos_libc_prf+0x2c6>
1c008d22:	01605f63          	blez	s6,1c008d40 <pos_libc_prf+0x2c6>
1c008d26:	fffb0d13          	addi	s10,s6,-1
1c008d2a:	85a6                	mv	a1,s1
1c008d2c:	02000513          	li	a0,32
1c008d30:	9902                	jalr	s2
1c008d32:	1d7d                	addi	s10,s10,-1
1c008d34:	e1f529e3          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008d38:	fffd39e3          	p.bneimm	s10,-1,1c008d2a <pos_libc_prf+0x2b0>
1c008d3c:	945a                	add	s0,s0,s6
1c008d3e:	5b7d                	li	s6,-1
1c008d40:	4792                	lw	a5,4(sp)
1c008d42:	00fc0d33          	add	s10,s8,a5
1c008d46:	a031                	j	1c008d52 <pos_libc_prf+0x2d8>
1c008d48:	001c450b          	p.lbu	a0,1(s8!)
1c008d4c:	9902                	jalr	s2
1c008d4e:	dff52ce3          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008d52:	85a6                	mv	a1,s1
1c008d54:	ffac1ae3          	bne	s8,s10,1c008d48 <pos_libc_prf+0x2ce>
1c008d58:	8d4e                	mv	s10,s3
1c008d5a:	a021                	j	1c008d62 <pos_libc_prf+0x2e8>
1c008d5c:	9902                	jalr	s2
1c008d5e:	dff524e3          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008d62:	1d7d                	addi	s10,s10,-1
1c008d64:	85a6                	mv	a1,s1
1c008d66:	03000513          	li	a0,48
1c008d6a:	fffd39e3          	p.bneimm	s10,-1,1c008d5c <pos_libc_prf+0x2e2>
1c008d6e:	4792                	lw	a5,4(sp)
1c008d70:	40fb8bb3          	sub	s7,s7,a5
1c008d74:	c45e                	sw	s7,8(sp)
1c008d76:	8d5e                	mv	s10,s7
1c008d78:	d80a9ce3          	bnez	s5,1c008b10 <pos_libc_prf+0x96>
1c008d7c:	47e2                	lw	a5,24(sp)
1c008d7e:	10079663          	bnez	a5,1c008e8a <pos_libc_prf+0x410>
1c008d82:	320d8be3          	beqz	s11,1c0098b8 <pos_libc_prf+0xe3e>
1c008d86:	000c4603          	lbu	a2,0(s8)
1c008d8a:	4aa5                	li	s5,9
1c008d8c:	02e00b93          	li	s7,46
1c008d90:	a801                	j	1c008da0 <pos_libc_prf+0x326>
1c008d92:	0c05                	addi	s8,s8,1
1c008d94:	9902                	jalr	s2
1c008d96:	dbf528e3          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c008d9a:	000c4603          	lbu	a2,0(s8)
1c008d9e:	1d7d                	addi	s10,s10,-1
1c008da0:	fd060813          	addi	a6,a2,-48 # 1a0fffd0 <__l1_end+0xa0fffcc>
1c008da4:	8532                	mv	a0,a2
1c008da6:	85a6                	mv	a1,s1
1c008da8:	ff0af5e3          	bleu	a6,s5,1c008d92 <pos_libc_prf+0x318>
1c008dac:	ff7603e3          	beq	a2,s7,1c008d92 <pos_libc_prf+0x318>
1c008db0:	8aee                	mv	s5,s11
1c008db2:	85a6                	mv	a1,s1
1c008db4:	03000513          	li	a0,48
1c008db8:	2f505ee3          	blez	s5,1c0098b4 <pos_libc_prf+0xe3a>
1c008dbc:	9902                	jalr	s2
1c008dbe:	1afd                	addi	s5,s5,-1
1c008dc0:	fff539e3          	p.bneimm	a0,-1,1c008db2 <pos_libc_prf+0x338>
1c008dc4:	547d                	li	s0,-1
1c008dc6:	b349                	j	1c008b48 <pos_libc_prf+0xce>
1c008dc8:	000ccc03          	lbu	s8,0(s9)
1c008dcc:	02a00713          	li	a4,42
1c008dd0:	001c8693          	addi	a3,s9,1
1c008dd4:	08ec0e63          	beq	s8,a4,1c008e70 <pos_libc_prf+0x3f6>
1c008dd8:	fd0c0793          	addi	a5,s8,-48
1c008ddc:	45a5                	li	a1,9
1c008dde:	8be2                	mv	s7,s8
1c008de0:	8736                	mv	a4,a3
1c008de2:	4981                	li	s3,0
1c008de4:	4625                	li	a2,9
1c008de6:	8cb6                	mv	s9,a3
1c008de8:	dcf5eae3          	bltu	a1,a5,1c008bbc <pos_libc_prf+0x142>
1c008dec:	00299793          	slli	a5,s3,0x2
1c008df0:	99be                	add	s3,s3,a5
1c008df2:	0986                	slli	s3,s3,0x1
1c008df4:	99e2                	add	s3,s3,s8
1c008df6:	00174c0b          	p.lbu	s8,1(a4!)
1c008dfa:	fd098993          	addi	s3,s3,-48
1c008dfe:	fd0c0693          	addi	a3,s8,-48
1c008e02:	8be2                	mv	s7,s8
1c008e04:	fed674e3          	bleu	a3,a2,1c008dec <pos_libc_prf+0x372>
1c008e08:	8cba                	mv	s9,a4
1c008e0a:	bb4d                	j	1c008bbc <pos_libc_prf+0x142>
1c008e0c:	000aab03          	lw	s6,0(s5)
1c008e10:	0a91                	addi	s5,s5,4
1c008e12:	060b4763          	bltz	s6,1c008e80 <pos_libc_prf+0x406>
1c008e16:	000bcc03          	lbu	s8,0(s7)
1c008e1a:	002d0c93          	addi	s9,s10,2
1c008e1e:	bb49                	j	1c008bb0 <pos_libc_prf+0x136>
1c008e20:	07500693          	li	a3,117
1c008e24:	0cdc0763          	beq	s8,a3,1c008ef2 <pos_libc_prf+0x478>
1c008e28:	07800693          	li	a3,120
1c008e2c:	0cdc0363          	beq	s8,a3,1c008ef2 <pos_libc_prf+0x478>
1c008e30:	07300713          	li	a4,115
1c008e34:	22ec13e3          	bne	s8,a4,1c00985a <pos_libc_prf+0xde0>
1c008e38:	004a8793          	addi	a5,s5,4
1c008e3c:	ce3e                	sw	a5,28(sp)
1c008e3e:	000aac03          	lw	s8,0(s5)
1c008e42:	3409c8e3          	bltz	s3,1c009992 <pos_libc_prf+0xf18>
1c008e46:	540985e3          	beqz	s3,1c009b90 <pos_libc_prf+0x1116>
1c008e4a:	000c4683          	lbu	a3,0(s8)
1c008e4e:	540680e3          	beqz	a3,1c009b8e <pos_libc_prf+0x1114>
1c008e52:	001c0693          	addi	a3,s8,1
1c008e56:	4b81                	li	s7,0
1c008e58:	a021                	j	1c008e60 <pos_libc_prf+0x3e6>
1c008e5a:	0016c60b          	p.lbu	a2,1(a3!)
1c008e5e:	c601                	beqz	a2,1c008e66 <pos_libc_prf+0x3ec>
1c008e60:	0b85                	addi	s7,s7,1
1c008e62:	ff3bcce3          	blt	s7,s3,1c008e5a <pos_libc_prf+0x3e0>
1c008e66:	417b0b33          	sub	s6,s6,s7
1c008e6a:	4981                	li	s3,0
1c008e6c:	c202                	sw	zero,4(sp)
1c008e6e:	b56d                	j	1c008d18 <pos_libc_prf+0x29e>
1c008e70:	001ccc03          	lbu	s8,1(s9)
1c008e74:	000aa983          	lw	s3,0(s5)
1c008e78:	0c89                	addi	s9,s9,2
1c008e7a:	8be2                	mv	s7,s8
1c008e7c:	0a91                	addi	s5,s5,4
1c008e7e:	bb3d                	j	1c008bbc <pos_libc_prf+0x142>
1c008e80:	4785                	li	a5,1
1c008e82:	41600b33          	neg	s6,s6
1c008e86:	c43e                	sw	a5,8(sp)
1c008e88:	b779                	j	1c008e16 <pos_libc_prf+0x39c>
1c008e8a:	02e00a93          	li	s5,46
1c008e8e:	a029                	j	1c008e98 <pos_libc_prf+0x41e>
1c008e90:	015b9463          	bne	s7,s5,1c008e98 <pos_libc_prf+0x41e>
1c008e94:	5ca0106f          	j	1c00a45e <pos_libc_prf+0x19e4>
1c008e98:	001c4b8b          	p.lbu	s7,1(s8!)
1c008e9c:	85a6                	mv	a1,s1
1c008e9e:	1d7d                	addi	s10,s10,-1
1c008ea0:	855e                	mv	a0,s7
1c008ea2:	9902                	jalr	s2
1c008ea4:	fff536e3          	p.bneimm	a0,-1,1c008e90 <pos_libc_prf+0x416>
1c008ea8:	547d                	li	s0,-1
1c008eaa:	b979                	j	1c008b48 <pos_libc_prf+0xce>
1c008eac:	1d7c05e3          	beq	s8,s7,1c009876 <pos_libc_prf+0xdfc>
1c008eb0:	8cb6                	mv	s9,a3
1c008eb2:	bb1d                	j	1c008be8 <pos_libc_prf+0x16e>
1c008eb4:	45a5                	li	a1,9
1c008eb6:	a019                	j	1c008ebc <pos_libc_prf+0x442>
1c008eb8:	8d5e                	mv	s10,s7
1c008eba:	0b85                	addi	s7,s7,1
1c008ebc:	000bcc03          	lbu	s8,0(s7)
1c008ec0:	002b1793          	slli	a5,s6,0x2
1c008ec4:	9b3e                	add	s6,s6,a5
1c008ec6:	0b06                	slli	s6,s6,0x1
1c008ec8:	9b6e                	add	s6,s6,s11
1c008eca:	fd0c0793          	addi	a5,s8,-48
1c008ece:	fd0b0b13          	addi	s6,s6,-48
1c008ed2:	8de2                	mv	s11,s8
1c008ed4:	fef5f2e3          	bleu	a5,a1,1c008eb8 <pos_libc_prf+0x43e>
1c008ed8:	002d0c93          	addi	s9,s10,2
1c008edc:	b9d1                	j	1c008bb0 <pos_libc_prf+0x136>
1c008ede:	06300693          	li	a3,99
1c008ee2:	72dc0963          	beq	s8,a3,1c009614 <pos_libc_prf+0xb9a>
1c008ee6:	d586efe3          	bltu	a3,s8,1c008c44 <pos_libc_prf+0x1ca>
1c008eea:	05800693          	li	a3,88
1c008eee:	16dc16e3          	bne	s8,a3,1c00985a <pos_libc_prf+0xde0>
1c008ef2:	06c00693          	li	a3,108
1c008ef6:	1ad707e3          	beq	a4,a3,1c0098a4 <pos_libc_prf+0xe2a>
1c008efa:	07a00693          	li	a3,122
1c008efe:	1ad703e3          	beq	a4,a3,1c0098a4 <pos_libc_prf+0xe2a>
1c008f02:	04c00693          	li	a3,76
1c008f06:	36d708e3          	beq	a4,a3,1c009a76 <pos_libc_prf+0xffc>
1c008f0a:	000aab83          	lw	s7,0(s5)
1c008f0e:	0a91                	addi	s5,s5,4
1c008f10:	ce56                	sw	s5,28(sp)
1c008f12:	4d01                	li	s10,0
1c008f14:	06f00713          	li	a4,111
1c008f18:	36ec0ce3          	beq	s8,a4,1c009a90 <pos_libc_prf+0x1016>
1c008f1c:	07500713          	li	a4,117
1c008f20:	02410a93          	addi	s5,sp,36
1c008f24:	04ec0a63          	beq	s8,a4,1c008f78 <pos_libc_prf+0x4fe>
1c008f28:	4792                	lw	a5,4(sp)
1c008f2a:	320786e3          	beqz	a5,1c009a56 <pos_libc_prf+0xfdc>
1c008f2e:	77e1                	lui	a5,0xffff8
1c008f30:	8307c793          	xori	a5,a5,-2000
1c008f34:	02f11223          	sh	a5,36(sp)
1c008f38:	4889                	li	a7,2
1c008f3a:	02610693          	addi	a3,sp,38
1c008f3e:	8736                	mv	a4,a3
1c008f40:	4e25                	li	t3,9
1c008f42:	f64bb633          	p.bclr	a2,s7,27,4
1c008f46:	01cd1593          	slli	a1,s10,0x1c
1c008f4a:	004bdb93          	srli	s7,s7,0x4
1c008f4e:	0175ebb3          	or	s7,a1,s7
1c008f52:	004d5d13          	srli	s10,s10,0x4
1c008f56:	05760e93          	addi	t4,a2,87
1c008f5a:	03060513          	addi	a0,a2,48
1c008f5e:	01abe333          	or	t1,s7,s10
1c008f62:	06ce7763          	bleu	a2,t3,1c008fd0 <pos_libc_prf+0x556>
1c008f66:	01d70023          	sb	t4,0(a4)
1c008f6a:	00170613          	addi	a2,a4,1
1c008f6e:	06030763          	beqz	t1,1c008fdc <pos_libc_prf+0x562>
1c008f72:	8732                	mv	a4,a2
1c008f74:	b7f9                	j	1c008f42 <pos_libc_prf+0x4c8>
1c008f76:	8aba                	mv	s5,a4
1c008f78:	4629                	li	a2,10
1c008f7a:	4681                	li	a3,0
1c008f7c:	855e                	mv	a0,s7
1c008f7e:	85ea                	mv	a1,s10
1c008f80:	c58ff0ef          	jal	ra,1c0083d8 <__umoddi3>
1c008f84:	8daa                	mv	s11,a0
1c008f86:	85ea                	mv	a1,s10
1c008f88:	855e                	mv	a0,s7
1c008f8a:	4681                	li	a3,0
1c008f8c:	4629                	li	a2,10
1c008f8e:	90eff0ef          	jal	ra,1c00809c <__udivdi3>
1c008f92:	030d8d93          	addi	s11,s11,48
1c008f96:	01ba8023          	sb	s11,0(s5)
1c008f9a:	00a5e6b3          	or	a3,a1,a0
1c008f9e:	001a8713          	addi	a4,s5,1
1c008fa2:	8baa                	mv	s7,a0
1c008fa4:	8d2e                	mv	s10,a1
1c008fa6:	fae1                	bnez	a3,1c008f76 <pos_libc_prf+0x4fc>
1c008fa8:	4782                	lw	a5,0(sp)
1c008faa:	00070023          	sb	zero,0(a4)
1c008fae:	40f70bb3          	sub	s7,a4,a5
1c008fb2:	0157fd63          	bleu	s5,a5,1c008fcc <pos_libc_prf+0x552>
1c008fb6:	1058                	addi	a4,sp,36
1c008fb8:	00074603          	lbu	a2,0(a4)
1c008fbc:	000ac683          	lbu	a3,0(s5)
1c008fc0:	feca8fab          	p.sb	a2,-1(s5!)
1c008fc4:	00d700ab          	p.sb	a3,1(a4!)
1c008fc8:	ff5768e3          	bltu	a4,s5,1c008fb8 <pos_libc_prf+0x53e>
1c008fcc:	c202                	sw	zero,4(sp)
1c008fce:	b33d                	j	1c008cfc <pos_libc_prf+0x282>
1c008fd0:	00a70023          	sb	a0,0(a4)
1c008fd4:	00170613          	addi	a2,a4,1
1c008fd8:	f8031de3          	bnez	t1,1c008f72 <pos_libc_prf+0x4f8>
1c008fdc:	00060023          	sb	zero,0(a2)
1c008fe0:	8e15                	sub	a2,a2,a3
1c008fe2:	00e6fc63          	bleu	a4,a3,1c008ffa <pos_libc_prf+0x580>
1c008fe6:	0006c503          	lbu	a0,0(a3)
1c008fea:	00074583          	lbu	a1,0(a4)
1c008fee:	fea70fab          	p.sb	a0,-1(a4!)
1c008ff2:	00b680ab          	p.sb	a1,1(a3!)
1c008ff6:	fee6e8e3          	bltu	a3,a4,1c008fe6 <pos_libc_prf+0x56c>
1c008ffa:	05800713          	li	a4,88
1c008ffe:	32ec06e3          	beq	s8,a4,1c009b2a <pos_libc_prf+0x10b0>
1c009002:	4792                	lw	a5,4(sp)
1c009004:	01160bb3          	add	s7,a2,a7
1c009008:	0786                	slli	a5,a5,0x1
1c00900a:	c23e                	sw	a5,4(sp)
1c00900c:	b9c5                	j	1c008cfc <pos_libc_prf+0x282>
1c00900e:	077c0be3          	beq	s8,s7,1c009884 <pos_libc_prf+0xe0a>
1c009012:	8cb6                	mv	s9,a3
1c009014:	06800713          	li	a4,104
1c009018:	bec1                	j	1c008be8 <pos_libc_prf+0x16e>
1c00901a:	4d81                	li	s11,0
1c00901c:	cc02                	sw	zero,24(sp)
1c00901e:	4a81                	li	s5,0
1c009020:	47d2                	lw	a5,20(sp)
1c009022:	5e078263          	beqz	a5,1c009606 <pos_libc_prf+0xb8c>
1c009026:	417b09b3          	sub	s3,s6,s7
1c00902a:	0409e9b3          	p.max	s3,s3,zero
1c00902e:	013b8733          	add	a4,s7,s3
1c009032:	40eb0b33          	sub	s6,s6,a4
1c009036:	02410c13          	addi	s8,sp,36
1c00903a:	b1d5                	j	1c008d1e <pos_libc_prf+0x2a4>
1c00903c:	4881                	li	a7,0
1c00903e:	4501                	li	a0,0
1c009040:	4701                	li	a4,0
1c009042:	4581                	li	a1,0
1c009044:	4e11                	li	t3,4
1c009046:	40ae0e33          	sub	t3,t3,a0
1c00904a:	011e40fb          	lp.setup	x1,t3,1c00906c <pos_libc_prf+0x5f2>
1c00904e:	01f59613          	slli	a2,a1,0x1f
1c009052:	00175693          	srli	a3,a4,0x1
1c009056:	8ed1                	or	a3,a3,a2
1c009058:	fc173733          	p.bclr	a4,a4,30,1
1c00905c:	96ba                	add	a3,a3,a4
1c00905e:	0015d613          	srli	a2,a1,0x1
1c009062:	00e6b733          	sltu	a4,a3,a4
1c009066:	963a                	add	a2,a2,a4
1c009068:	85b2                	mv	a1,a2
1c00906a:	8736                	mv	a4,a3
1c00906c:	0505                	addi	a0,a0,1
1c00906e:	0009d363          	bgez	s3,1c009074 <pos_libc_prf+0x5fa>
1c009072:	4999                	li	s3,6
1c009074:	04700713          	li	a4,71
1c009078:	16ee8be3          	beq	t4,a4,1c0099ee <pos_libc_prf+0xf74>
1c00907c:	06600713          	li	a4,102
1c009080:	4581                	li	a1,0
1c009082:	18ec04e3          	beq	s8,a4,1c009a0a <pos_libc_prf+0xf90>
1c009086:	00198e13          	addi	t3,s3,1
1c00908a:	47c1                	li	a5,16
1c00908c:	04fe4e33          	p.min	t3,t3,a5
1c009090:	1e7d                	addi	t3,t3,-1
1c009092:	4701                	li	a4,0
1c009094:	08000fb7          	lui	t6,0x8000
1c009098:	4e95                	li	t4,5
1c00909a:	0e05                	addi	t3,t3,1
1c00909c:	032e40fb          	lp.setup	x1,t3,1c009100 <pos_libc_prf+0x686>
1c0090a0:	00270f13          	addi	t5,a4,2
1c0090a4:	00ef3733          	sltu	a4,t5,a4
1c0090a8:	977e                	add	a4,a4,t6
1c0090aa:	03d752b3          	divu	t0,a4,t4
1c0090ae:	003f5f93          	srli	t6,t5,0x3
1c0090b2:	00229393          	slli	t2,t0,0x2
1c0090b6:	9396                	add	t2,t2,t0
1c0090b8:	40770733          	sub	a4,a4,t2
1c0090bc:	0776                	slli	a4,a4,0x1d
1c0090be:	01f76fb3          	or	t6,a4,t6
1c0090c2:	03dfdfb3          	divu	t6,t6,t4
1c0090c6:	003f9393          	slli	t2,t6,0x3
1c0090ca:	005f9713          	slli	a4,t6,0x5
1c0090ce:	971e                	add	a4,a4,t2
1c0090d0:	40ef0733          	sub	a4,t5,a4
1c0090d4:	03d75733          	divu	a4,a4,t4
1c0090d8:	01dfdf93          	srli	t6,t6,0x1d
1c0090dc:	92fe                	add	t0,t0,t6
1c0090de:	93ba                	add	t2,t2,a4
1c0090e0:	00e3b733          	sltu	a4,t2,a4
1c0090e4:	92ba                	add	t0,t0,a4
1c0090e6:	01f29f13          	slli	t5,t0,0x1f
1c0090ea:	0013d713          	srli	a4,t2,0x1
1c0090ee:	00ef6733          	or	a4,t5,a4
1c0090f2:	fc13b3b3          	p.bclr	t2,t2,30,1
1c0090f6:	971e                	add	a4,a4,t2
1c0090f8:	0012df13          	srli	t5,t0,0x1
1c0090fc:	00773fb3          	sltu	t6,a4,t2
1c009100:	9ffa                	add	t6,t6,t5
1c009102:	9736                	add	a4,a4,a3
1c009104:	967e                	add	a2,a2,t6
1c009106:	00d736b3          	sltu	a3,a4,a3
1c00910a:	96b2                	add	a3,a3,a2
1c00910c:	f606b633          	p.bclr	a2,a3,27,0
1c009110:	c225                	beqz	a2,1c009170 <pos_libc_prf+0x6f6>
1c009112:	00270e13          	addi	t3,a4,2
1c009116:	00ee3733          	sltu	a4,t3,a4
1c00911a:	9736                	add	a4,a4,a3
1c00911c:	4f15                	li	t5,5
1c00911e:	03e75eb3          	divu	t4,a4,t5
1c009122:	003e5693          	srli	a3,t3,0x3
1c009126:	0885                	addi	a7,a7,1
1c009128:	002e9613          	slli	a2,t4,0x2
1c00912c:	9676                	add	a2,a2,t4
1c00912e:	8f11                	sub	a4,a4,a2
1c009130:	0776                	slli	a4,a4,0x1d
1c009132:	8ed9                	or	a3,a3,a4
1c009134:	03e6d6b3          	divu	a3,a3,t5
1c009138:	00369613          	slli	a2,a3,0x3
1c00913c:	00569713          	slli	a4,a3,0x5
1c009140:	9732                	add	a4,a4,a2
1c009142:	40ee0733          	sub	a4,t3,a4
1c009146:	03e75733          	divu	a4,a4,t5
1c00914a:	82f5                	srli	a3,a3,0x1d
1c00914c:	96f6                	add	a3,a3,t4
1c00914e:	963a                	add	a2,a2,a4
1c009150:	00e63733          	sltu	a4,a2,a4
1c009154:	96ba                	add	a3,a3,a4
1c009156:	01f69e13          	slli	t3,a3,0x1f
1c00915a:	00165713          	srli	a4,a2,0x1
1c00915e:	00ee6733          	or	a4,t3,a4
1c009162:	fc163633          	p.bclr	a2,a2,30,1
1c009166:	9732                	add	a4,a4,a2
1c009168:	8285                	srli	a3,a3,0x1
1c00916a:	00c73633          	sltu	a2,a4,a2
1c00916e:	96b2                	add	a3,a3,a2
1c009170:	06600613          	li	a2,102
1c009174:	00130e13          	addi	t3,t1,1
1c009178:	22cc0be3          	beq	s8,a2,1c009bae <pos_libc_prf+0x1134>
1c00917c:	01e75e93          	srli	t4,a4,0x1e
1c009180:	00271613          	slli	a2,a4,0x2
1c009184:	00269513          	slli	a0,a3,0x2
1c009188:	9732                	add	a4,a4,a2
1c00918a:	00aee533          	or	a0,t4,a0
1c00918e:	00c73633          	sltu	a2,a4,a2
1c009192:	96aa                	add	a3,a3,a0
1c009194:	96b2                	add	a3,a3,a2
1c009196:	0686                	slli	a3,a3,0x1
1c009198:	01f75613          	srli	a2,a4,0x1f
1c00919c:	8ed1                	or	a3,a3,a2
1c00919e:	01c6d613          	srli	a2,a3,0x1c
1c0091a2:	4792                	lw	a5,4(sp)
1c0091a4:	8eb2                	mv	t4,a2
1c0091a6:	03060613          	addi	a2,a2,48
1c0091aa:	01d03eb3          	snez	t4,t4
1c0091ae:	00c30023          	sb	a2,0(t1)
1c0091b2:	c7c6b533          	p.bclr	a0,a3,3,28
1c0091b6:	00171613          	slli	a2,a4,0x1
1c0091ba:	41d888b3          	sub	a7,a7,t4
1c0091be:	1c079de3          	bnez	a5,1c009b98 <pos_libc_prf+0x111e>
1c0091c2:	8dce                	mv	s11,s3
1c0091c4:	3d305e63          	blez	s3,1c0095a0 <pos_libc_prf+0xb26>
1c0091c8:	02e00693          	li	a3,46
1c0091cc:	00230713          	addi	a4,t1,2
1c0091d0:	00d300a3          	sb	a3,1(t1)
1c0091d4:	00261693          	slli	a3,a2,0x2
1c0091d8:	01e65313          	srli	t1,a2,0x1e
1c0091dc:	00251e13          	slli	t3,a0,0x2
1c0091e0:	9636                	add	a2,a2,a3
1c0091e2:	01c36e33          	or	t3,t1,t3
1c0091e6:	00d63333          	sltu	t1,a2,a3
1c0091ea:	00ae06b3          	add	a3,t3,a0
1c0091ee:	9336                	add	t1,t1,a3
1c0091f0:	0306                	slli	t1,t1,0x1
1c0091f2:	01f65693          	srli	a3,a2,0x1f
1c0091f6:	0066e333          	or	t1,a3,t1
1c0091fa:	01c35e13          	srli	t3,t1,0x1c
1c0091fe:	030e0e13          	addi	t3,t3,48
1c009202:	100006b7          	lui	a3,0x10000
1c009206:	fff68513          	addi	a0,a3,-1 # fffffff <__l1_heap_size+0xfff0003>
1c00920a:	01c70023          	sb	t3,0(a4)
1c00920e:	00161e93          	slli	t4,a2,0x1
1c009212:	00a37333          	and	t1,t1,a0
1c009216:	00170e13          	addi	t3,a4,1
1c00921a:	4d81                	li	s11,0
1c00921c:	3819a263          	p.beqimm	s3,1,1c0095a0 <pos_libc_prf+0xb26>
1c009220:	00361693          	slli	a3,a2,0x3
1c009224:	00231e13          	slli	t3,t1,0x2
1c009228:	01eedf13          	srli	t5,t4,0x1e
1c00922c:	01d68633          	add	a2,a3,t4
1c009230:	01cf6e33          	or	t3,t5,t3
1c009234:	9372                	add	t1,t1,t3
1c009236:	00d636b3          	sltu	a3,a2,a3
1c00923a:	969a                	add	a3,a3,t1
1c00923c:	0686                	slli	a3,a3,0x1
1c00923e:	01f65313          	srli	t1,a2,0x1f
1c009242:	00d366b3          	or	a3,t1,a3
1c009246:	01c6d313          	srli	t1,a3,0x1c
1c00924a:	03030313          	addi	t1,t1,48
1c00924e:	006700a3          	sb	t1,1(a4)
1c009252:	ffe98d93          	addi	s11,s3,-2
1c009256:	00161e13          	slli	t3,a2,0x1
1c00925a:	8ee9                	and	a3,a3,a0
1c00925c:	01b04463          	bgtz	s11,1c009264 <pos_libc_prf+0x7ea>
1c009260:	37a0106f          	j	1c00a5da <pos_libc_prf+0x1b60>
1c009264:	01ee5e93          	srli	t4,t3,0x1e
1c009268:	060e                	slli	a2,a2,0x3
1c00926a:	00269313          	slli	t1,a3,0x2
1c00926e:	9e32                	add	t3,t3,a2
1c009270:	006ee333          	or	t1,t4,t1
1c009274:	969a                	add	a3,a3,t1
1c009276:	00ce3633          	sltu	a2,t3,a2
1c00927a:	9636                	add	a2,a2,a3
1c00927c:	0606                	slli	a2,a2,0x1
1c00927e:	01fe5693          	srli	a3,t3,0x1f
1c009282:	8e55                	or	a2,a2,a3
1c009284:	01c65693          	srli	a3,a2,0x1c
1c009288:	03068693          	addi	a3,a3,48
1c00928c:	00d70123          	sb	a3,2(a4)
1c009290:	ffd98d93          	addi	s11,s3,-3
1c009294:	001e1e93          	slli	t4,t3,0x1
1c009298:	8e69                	and	a2,a2,a0
1c00929a:	01b04463          	bgtz	s11,1c0092a2 <pos_libc_prf+0x828>
1c00929e:	3340106f          	j	1c00a5d2 <pos_libc_prf+0x1b58>
1c0092a2:	003e1693          	slli	a3,t3,0x3
1c0092a6:	00261313          	slli	t1,a2,0x2
1c0092aa:	01eede13          	srli	t3,t4,0x1e
1c0092ae:	006e6333          	or	t1,t3,t1
1c0092b2:	9eb6                	add	t4,t4,a3
1c0092b4:	961a                	add	a2,a2,t1
1c0092b6:	00deb6b3          	sltu	a3,t4,a3
1c0092ba:	96b2                	add	a3,a3,a2
1c0092bc:	0686                	slli	a3,a3,0x1
1c0092be:	01fed613          	srli	a2,t4,0x1f
1c0092c2:	8ed1                	or	a3,a3,a2
1c0092c4:	01c6d613          	srli	a2,a3,0x1c
1c0092c8:	03060613          	addi	a2,a2,48
1c0092cc:	00c701a3          	sb	a2,3(a4)
1c0092d0:	ffc98d93          	addi	s11,s3,-4
1c0092d4:	001e9e13          	slli	t3,t4,0x1
1c0092d8:	8ee9                	and	a3,a3,a0
1c0092da:	01b04463          	bgtz	s11,1c0092e2 <pos_libc_prf+0x868>
1c0092de:	2ec0106f          	j	1c00a5ca <pos_libc_prf+0x1b50>
1c0092e2:	003e9613          	slli	a2,t4,0x3
1c0092e6:	00269313          	slli	t1,a3,0x2
1c0092ea:	01ee5e93          	srli	t4,t3,0x1e
1c0092ee:	006ee333          	or	t1,t4,t1
1c0092f2:	9e32                	add	t3,t3,a2
1c0092f4:	969a                	add	a3,a3,t1
1c0092f6:	00ce3633          	sltu	a2,t3,a2
1c0092fa:	9636                	add	a2,a2,a3
1c0092fc:	0606                	slli	a2,a2,0x1
1c0092fe:	01fe5693          	srli	a3,t3,0x1f
1c009302:	8e55                	or	a2,a2,a3
1c009304:	01c65693          	srli	a3,a2,0x1c
1c009308:	03068693          	addi	a3,a3,48
1c00930c:	00d70223          	sb	a3,4(a4)
1c009310:	ffb98d93          	addi	s11,s3,-5
1c009314:	001e1e93          	slli	t4,t3,0x1
1c009318:	8e69                	and	a2,a2,a0
1c00931a:	01b04463          	bgtz	s11,1c009322 <pos_libc_prf+0x8a8>
1c00931e:	2a40106f          	j	1c00a5c2 <pos_libc_prf+0x1b48>
1c009322:	003e1693          	slli	a3,t3,0x3
1c009326:	00261313          	slli	t1,a2,0x2
1c00932a:	01eede13          	srli	t3,t4,0x1e
1c00932e:	006e6333          	or	t1,t3,t1
1c009332:	9eb6                	add	t4,t4,a3
1c009334:	961a                	add	a2,a2,t1
1c009336:	00deb6b3          	sltu	a3,t4,a3
1c00933a:	96b2                	add	a3,a3,a2
1c00933c:	0686                	slli	a3,a3,0x1
1c00933e:	01fed613          	srli	a2,t4,0x1f
1c009342:	8ed1                	or	a3,a3,a2
1c009344:	01c6d613          	srli	a2,a3,0x1c
1c009348:	03060613          	addi	a2,a2,48
1c00934c:	00c702a3          	sb	a2,5(a4)
1c009350:	ffa98d93          	addi	s11,s3,-6
1c009354:	001e9e13          	slli	t3,t4,0x1
1c009358:	8ee9                	and	a3,a3,a0
1c00935a:	01b04463          	bgtz	s11,1c009362 <pos_libc_prf+0x8e8>
1c00935e:	25c0106f          	j	1c00a5ba <pos_libc_prf+0x1b40>
1c009362:	003e9613          	slli	a2,t4,0x3
1c009366:	00269313          	slli	t1,a3,0x2
1c00936a:	01ee5e93          	srli	t4,t3,0x1e
1c00936e:	006ee333          	or	t1,t4,t1
1c009372:	9e32                	add	t3,t3,a2
1c009374:	969a                	add	a3,a3,t1
1c009376:	00ce3633          	sltu	a2,t3,a2
1c00937a:	9636                	add	a2,a2,a3
1c00937c:	0606                	slli	a2,a2,0x1
1c00937e:	01fe5693          	srli	a3,t3,0x1f
1c009382:	8e55                	or	a2,a2,a3
1c009384:	01c65693          	srli	a3,a2,0x1c
1c009388:	03068693          	addi	a3,a3,48
1c00938c:	00d70323          	sb	a3,6(a4)
1c009390:	ff998d93          	addi	s11,s3,-7
1c009394:	001e1e93          	slli	t4,t3,0x1
1c009398:	8e69                	and	a2,a2,a0
1c00939a:	01b04463          	bgtz	s11,1c0093a2 <pos_libc_prf+0x928>
1c00939e:	2140106f          	j	1c00a5b2 <pos_libc_prf+0x1b38>
1c0093a2:	003e1693          	slli	a3,t3,0x3
1c0093a6:	00261313          	slli	t1,a2,0x2
1c0093aa:	01eede13          	srli	t3,t4,0x1e
1c0093ae:	006e6333          	or	t1,t3,t1
1c0093b2:	9eb6                	add	t4,t4,a3
1c0093b4:	961a                	add	a2,a2,t1
1c0093b6:	00deb6b3          	sltu	a3,t4,a3
1c0093ba:	96b2                	add	a3,a3,a2
1c0093bc:	0686                	slli	a3,a3,0x1
1c0093be:	01fed613          	srli	a2,t4,0x1f
1c0093c2:	8ed1                	or	a3,a3,a2
1c0093c4:	01c6d613          	srli	a2,a3,0x1c
1c0093c8:	03060613          	addi	a2,a2,48
1c0093cc:	00c703a3          	sb	a2,7(a4)
1c0093d0:	ff898d93          	addi	s11,s3,-8
1c0093d4:	001e9e13          	slli	t3,t4,0x1
1c0093d8:	8ee9                	and	a3,a3,a0
1c0093da:	01b04463          	bgtz	s11,1c0093e2 <pos_libc_prf+0x968>
1c0093de:	1cc0106f          	j	1c00a5aa <pos_libc_prf+0x1b30>
1c0093e2:	003e9613          	slli	a2,t4,0x3
1c0093e6:	00269313          	slli	t1,a3,0x2
1c0093ea:	01ee5e93          	srli	t4,t3,0x1e
1c0093ee:	006ee333          	or	t1,t4,t1
1c0093f2:	9e32                	add	t3,t3,a2
1c0093f4:	969a                	add	a3,a3,t1
1c0093f6:	00ce3633          	sltu	a2,t3,a2
1c0093fa:	9636                	add	a2,a2,a3
1c0093fc:	0606                	slli	a2,a2,0x1
1c0093fe:	01fe5693          	srli	a3,t3,0x1f
1c009402:	8e55                	or	a2,a2,a3
1c009404:	01c65693          	srli	a3,a2,0x1c
1c009408:	03068693          	addi	a3,a3,48
1c00940c:	00d70423          	sb	a3,8(a4)
1c009410:	ff798d93          	addi	s11,s3,-9
1c009414:	001e1e93          	slli	t4,t3,0x1
1c009418:	8e69                	and	a2,a2,a0
1c00941a:	01b04463          	bgtz	s11,1c009422 <pos_libc_prf+0x9a8>
1c00941e:	1840106f          	j	1c00a5a2 <pos_libc_prf+0x1b28>
1c009422:	003e1693          	slli	a3,t3,0x3
1c009426:	00261313          	slli	t1,a2,0x2
1c00942a:	01eedf13          	srli	t5,t4,0x1e
1c00942e:	01d68e33          	add	t3,a3,t4
1c009432:	006f6333          	or	t1,t5,t1
1c009436:	00de36b3          	sltu	a3,t3,a3
1c00943a:	9332                	add	t1,t1,a2
1c00943c:	969a                	add	a3,a3,t1
1c00943e:	01fe5613          	srli	a2,t3,0x1f
1c009442:	0686                	slli	a3,a3,0x1
1c009444:	8ed1                	or	a3,a3,a2
1c009446:	01c6d613          	srli	a2,a3,0x1c
1c00944a:	03060613          	addi	a2,a2,48
1c00944e:	00c704a3          	sb	a2,9(a4)
1c009452:	ff698d93          	addi	s11,s3,-10
1c009456:	001e1613          	slli	a2,t3,0x1
1c00945a:	8ee9                	and	a3,a3,a0
1c00945c:	00a70e13          	addi	t3,a4,10
1c009460:	15b05063          	blez	s11,1c0095a0 <pos_libc_prf+0xb26>
1c009464:	00261513          	slli	a0,a2,0x2
1c009468:	01e65313          	srli	t1,a2,0x1e
1c00946c:	00269e13          	slli	t3,a3,0x2
1c009470:	962a                	add	a2,a2,a0
1c009472:	01c36e33          	or	t3,t1,t3
1c009476:	96f2                	add	a3,a3,t3
1c009478:	00a63333          	sltu	t1,a2,a0
1c00947c:	9336                	add	t1,t1,a3
1c00947e:	0306                	slli	t1,t1,0x1
1c009480:	01f65693          	srli	a3,a2,0x1f
1c009484:	0066e333          	or	t1,a3,t1
1c009488:	01c35e13          	srli	t3,t1,0x1c
1c00948c:	030e0e13          	addi	t3,t3,48
1c009490:	100006b7          	lui	a3,0x10000
1c009494:	fff68513          	addi	a0,a3,-1 # fffffff <__l1_heap_size+0xfff0003>
1c009498:	01c70523          	sb	t3,10(a4)
1c00949c:	ff598d93          	addi	s11,s3,-11
1c0094a0:	00161e93          	slli	t4,a2,0x1
1c0094a4:	00a37333          	and	t1,t1,a0
1c0094a8:	00b70e13          	addi	t3,a4,11
1c0094ac:	0fb05a63          	blez	s11,1c0095a0 <pos_libc_prf+0xb26>
1c0094b0:	00361693          	slli	a3,a2,0x3
1c0094b4:	00231e13          	slli	t3,t1,0x2
1c0094b8:	01eedf13          	srli	t5,t4,0x1e
1c0094bc:	01d68633          	add	a2,a3,t4
1c0094c0:	01cf6e33          	or	t3,t5,t3
1c0094c4:	9372                	add	t1,t1,t3
1c0094c6:	00d636b3          	sltu	a3,a2,a3
1c0094ca:	969a                	add	a3,a3,t1
1c0094cc:	0686                	slli	a3,a3,0x1
1c0094ce:	01f65313          	srli	t1,a2,0x1f
1c0094d2:	00d366b3          	or	a3,t1,a3
1c0094d6:	01c6d313          	srli	t1,a3,0x1c
1c0094da:	03030313          	addi	t1,t1,48
1c0094de:	006705a3          	sb	t1,11(a4)
1c0094e2:	ff498d93          	addi	s11,s3,-12
1c0094e6:	00161e13          	slli	t3,a2,0x1
1c0094ea:	8ee9                	and	a3,a3,a0
1c0094ec:	01b04463          	bgtz	s11,1c0094f4 <pos_libc_prf+0xa7a>
1c0094f0:	09c0106f          	j	1c00a58c <pos_libc_prf+0x1b12>
1c0094f4:	060e                	slli	a2,a2,0x3
1c0094f6:	01ee5e93          	srli	t4,t3,0x1e
1c0094fa:	00269313          	slli	t1,a3,0x2
1c0094fe:	9e32                	add	t3,t3,a2
1c009500:	006ee333          	or	t1,t4,t1
1c009504:	969a                	add	a3,a3,t1
1c009506:	00ce3633          	sltu	a2,t3,a2
1c00950a:	9636                	add	a2,a2,a3
1c00950c:	0606                	slli	a2,a2,0x1
1c00950e:	01fe5693          	srli	a3,t3,0x1f
1c009512:	8e55                	or	a2,a2,a3
1c009514:	01c65693          	srli	a3,a2,0x1c
1c009518:	03068693          	addi	a3,a3,48
1c00951c:	00d70623          	sb	a3,12(a4)
1c009520:	ff398d93          	addi	s11,s3,-13
1c009524:	001e1313          	slli	t1,t3,0x1
1c009528:	01b04463          	bgtz	s11,1c009530 <pos_libc_prf+0xab6>
1c00952c:	0580106f          	j	1c00a584 <pos_libc_prf+0x1b0a>
1c009530:	8e69                	and	a2,a2,a0
1c009532:	003e1693          	slli	a3,t3,0x3
1c009536:	01e35e93          	srli	t4,t1,0x1e
1c00953a:	00261e13          	slli	t3,a2,0x2
1c00953e:	9336                	add	t1,t1,a3
1c009540:	01ceee33          	or	t3,t4,t3
1c009544:	9672                	add	a2,a2,t3
1c009546:	00d336b3          	sltu	a3,t1,a3
1c00954a:	96b2                	add	a3,a3,a2
1c00954c:	0686                	slli	a3,a3,0x1
1c00954e:	01f35613          	srli	a2,t1,0x1f
1c009552:	8ed1                	or	a3,a3,a2
1c009554:	01c6d613          	srli	a2,a3,0x1c
1c009558:	03060613          	addi	a2,a2,48
1c00955c:	00c706a3          	sb	a2,13(a4)
1c009560:	ff298d93          	addi	s11,s3,-14
1c009564:	00131613          	slli	a2,t1,0x1
1c009568:	8ee9                	and	a3,a3,a0
1c00956a:	00e70e13          	addi	t3,a4,14
1c00956e:	03b05963          	blez	s11,1c0095a0 <pos_libc_prf+0xb26>
1c009572:	00331513          	slli	a0,t1,0x3
1c009576:	01e65e13          	srli	t3,a2,0x1e
1c00957a:	00269313          	slli	t1,a3,0x2
1c00957e:	006e6333          	or	t1,t3,t1
1c009582:	962a                	add	a2,a2,a0
1c009584:	00a63633          	sltu	a2,a2,a0
1c009588:	969a                	add	a3,a3,t1
1c00958a:	96b2                	add	a3,a3,a2
1c00958c:	0686                	slli	a3,a3,0x1
1c00958e:	82f1                	srli	a3,a3,0x1c
1c009590:	03068693          	addi	a3,a3,48
1c009594:	00f70e13          	addi	t3,a4,15
1c009598:	00d70723          	sb	a3,14(a4)
1c00959c:	ff198d93          	addi	s11,s3,-15
1c0095a0:	4a058763          	beqz	a1,1c009a4e <pos_libc_prf+0xfd4>
1c0095a4:	4b81                	li	s7,0
1c0095a6:	cc02                	sw	zero,24(sp)
1c0095a8:	4a81                	li	s5,0
1c0095aa:	03000613          	li	a2,48
1c0095ae:	a011                	j	1c0095b2 <pos_libc_prf+0xb38>
1c0095b0:	8e3a                	mv	t3,a4
1c0095b2:	fffe0713          	addi	a4,t3,-1
1c0095b6:	00074683          	lbu	a3,0(a4)
1c0095ba:	fec68be3          	beq	a3,a2,1c0095b0 <pos_libc_prf+0xb36>
1c0095be:	02e00613          	li	a2,46
1c0095c2:	4d81                	li	s11,0
1c0095c4:	00c69363          	bne	a3,a2,1c0095ca <pos_libc_prf+0xb50>
1c0095c8:	8e3a                	mv	t3,a4
1c0095ca:	0dfc7713          	andi	a4,s8,223
1c0095ce:	04500693          	li	a3,69
1c0095d2:	3cd70663          	beq	a4,a3,1c00999e <pos_libc_prf+0xf24>
1c0095d6:	4782                	lw	a5,0(sp)
1c0095d8:	000e0023          	sb	zero,0(t3)
1c0095dc:	40fe0e33          	sub	t3,t3,a5
1c0095e0:	47b2                	lw	a5,12(sp)
1c0095e2:	2a079863          	bnez	a5,1c009892 <pos_libc_prf+0xe18>
1c0095e6:	47c2                	lw	a5,16(sp)
1c0095e8:	2a079563          	bnez	a5,1c009892 <pos_libc_prf+0xe18>
1c0095ec:	02414703          	lbu	a4,36(sp)
1c0095f0:	02d00693          	li	a3,45
1c0095f4:	c202                	sw	zero,4(sp)
1c0095f6:	28d70e63          	beq	a4,a3,1c009892 <pos_libc_prf+0xe18>
1c0095fa:	fd070713          	addi	a4,a4,-48
1c0095fe:	46a5                	li	a3,9
1c009600:	9bf2                	add	s7,s7,t3
1c009602:	a0e6ffe3          	bleu	a4,a3,1c009020 <pos_libc_prf+0x5a6>
1c009606:	417b0b33          	sub	s6,s6,s7
1c00960a:	02410c13          	addi	s8,sp,36
1c00960e:	4981                	li	s3,0
1c009610:	f0eff06f          	j	1c008d1e <pos_libc_prf+0x2a4>
1c009614:	000aa703          	lw	a4,0(s5)
1c009618:	0a91                	addi	s5,s5,4
1c00961a:	ce56                	sw	s5,28(sp)
1c00961c:	1b7d                	addi	s6,s6,-1
1c00961e:	02e10223          	sb	a4,36(sp)
1c009622:	4981                	li	s3,0
1c009624:	c202                	sw	zero,4(sp)
1c009626:	4b85                	li	s7,1
1c009628:	02410c13          	addi	s8,sp,36
1c00962c:	eecff06f          	j	1c008d18 <pos_libc_prf+0x29e>
1c009630:	007a8693          	addi	a3,s5,7
1c009634:	c406b6b3          	p.bclr	a3,a3,2,0
1c009638:	0046a883          	lw	a7,4(a3)
1c00963c:	4298                	lw	a4,0(a3)
1c00963e:	06a1                	addi	a3,a3,8
1c009640:	ce36                	sw	a3,28(sp)
1c009642:	00b89613          	slli	a2,a7,0xb
1c009646:	01575693          	srli	a3,a4,0x15
1c00964a:	0148d513          	srli	a0,a7,0x14
1c00964e:	8e55                	or	a2,a2,a3
1c009650:	e8b53533          	p.bclr	a0,a0,20,11
1c009654:	072e                	slli	a4,a4,0xb
1c009656:	c1f635b3          	p.bclr	a1,a2,0,31
1c00965a:	2a08cd63          	bltz	a7,1c009914 <pos_libc_prf+0xe9a>
1c00965e:	47b2                	lw	a5,12(sp)
1c009660:	28078e63          	beqz	a5,1c0098fc <pos_libc_prf+0xe82>
1c009664:	02b00693          	li	a3,43
1c009668:	02d10223          	sb	a3,36(sp)
1c00966c:	02510313          	addi	t1,sp,37
1c009670:	7ff00693          	li	a3,2047
1c009674:	2ed50263          	beq	a0,a3,1c009958 <pos_libc_prf+0xede>
1c009678:	04600693          	li	a3,70
1c00967c:	0dfc7e93          	andi	t4,s8,223
1c009680:	50dc0263          	beq	s8,a3,1c009b84 <pos_libc_prf+0x110a>
1c009684:	41f55613          	srai	a2,a0,0x1f
1c009688:	00e566b3          	or	a3,a0,a4
1c00968c:	8e4d                	or	a2,a2,a1
1c00968e:	8ed1                	or	a3,a3,a2
1c009690:	9a0686e3          	beqz	a3,1c00903c <pos_libc_prf+0x5c2>
1c009694:	e505                	bnez	a0,1c0096bc <pos_libc_prf+0xc42>
1c009696:	01f75693          	srli	a3,a4,0x1f
1c00969a:	00159613          	slli	a2,a1,0x1
1c00969e:	00c6e5b3          	or	a1,a3,a2
1c0096a2:	0706                	slli	a4,a4,0x1
1c0096a4:	6e05c8e3          	bltz	a1,1c00a594 <pos_libc_prf+0x1b1a>
1c0096a8:	01f75693          	srli	a3,a4,0x1f
1c0096ac:	00159613          	slli	a2,a1,0x1
1c0096b0:	00c6e5b3          	or	a1,a3,a2
1c0096b4:	157d                	addi	a0,a0,-1
1c0096b6:	0706                	slli	a4,a4,0x1
1c0096b8:	fe05d8e3          	bgez	a1,1c0096a8 <pos_libc_prf+0xc2e>
1c0096bc:	c0250513          	addi	a0,a0,-1022
1c0096c0:	56f9                	li	a3,-2
1c0096c2:	c1f5c5b3          	p.bset	a1,a1,0,31
1c0096c6:	4881                	li	a7,0
1c0096c8:	06d55a63          	ble	a3,a0,1c00973c <pos_libc_prf+0xcc2>
1c0096cc:	33333e37          	lui	t3,0x33333
1c0096d0:	80000f37          	lui	t5,0x80000
1c0096d4:	4881                	li	a7,0
1c0096d6:	332e0e13          	addi	t3,t3,818 # 33333332 <__l2_shared_end+0x173232fa>
1c0096da:	ffff4f13          	not	t5,t5
1c0096de:	5ff9                	li	t6,-2
1c0096e0:	a011                	j	1c0096e4 <pos_libc_prf+0xc6a>
1c0096e2:	8536                	mv	a0,a3
1c0096e4:	01f59293          	slli	t0,a1,0x1f
1c0096e8:	00175693          	srli	a3,a4,0x1
1c0096ec:	fc173633          	p.bclr	a2,a4,30,1
1c0096f0:	00d2e6b3          	or	a3,t0,a3
1c0096f4:	00d60733          	add	a4,a2,a3
1c0096f8:	8185                	srli	a1,a1,0x1
1c0096fa:	00c73633          	sltu	a2,a4,a2
1c0096fe:	95b2                	add	a1,a1,a2
1c009700:	00150693          	addi	a3,a0,1
1c009704:	fcbe6fe3          	bltu	t3,a1,1c0096e2 <pos_libc_prf+0xc68>
1c009708:	00271293          	slli	t0,a4,0x2
1c00970c:	01e75393          	srli	t2,a4,0x1e
1c009710:	00259613          	slli	a2,a1,0x2
1c009714:	9716                	add	a4,a4,t0
1c009716:	00c3e633          	or	a2,t2,a2
1c00971a:	962e                	add	a2,a2,a1
1c00971c:	005732b3          	sltu	t0,a4,t0
1c009720:	00c285b3          	add	a1,t0,a2
1c009724:	0509                	addi	a0,a0,2
1c009726:	01f75613          	srli	a2,a4,0x1f
1c00972a:	18fd                	addi	a7,a7,-1
1c00972c:	01e59663          	bne	a1,t5,1c009738 <pos_libc_prf+0xcbe>
1c009730:	0706                	slli	a4,a4,0x1
1c009732:	fc1645b3          	p.bset	a1,a2,30,1
1c009736:	8536                	mv	a0,a3
1c009738:	fbf546e3          	blt	a0,t6,1c0096e4 <pos_libc_prf+0xc6a>
1c00973c:	80000e37          	lui	t3,0x80000
1c009740:	4f15                	li	t5,5
1c009742:	fffe4e13          	not	t3,t3
1c009746:	8ea05fe3          	blez	a0,1c009044 <pos_libc_prf+0x5ca>
1c00974a:	00270693          	addi	a3,a4,2
1c00974e:	00e6b733          	sltu	a4,a3,a4
1c009752:	00b70633          	add	a2,a4,a1
1c009756:	03e655b3          	divu	a1,a2,t5
1c00975a:	0036df93          	srli	t6,a3,0x3
1c00975e:	157d                	addi	a0,a0,-1
1c009760:	0885                	addi	a7,a7,1
1c009762:	00259713          	slli	a4,a1,0x2
1c009766:	972e                	add	a4,a4,a1
1c009768:	40e60733          	sub	a4,a2,a4
1c00976c:	0776                	slli	a4,a4,0x1d
1c00976e:	01f76fb3          	or	t6,a4,t6
1c009772:	03efd633          	divu	a2,t6,t5
1c009776:	00361713          	slli	a4,a2,0x3
1c00977a:	00561f93          	slli	t6,a2,0x5
1c00977e:	9fba                	add	t6,t6,a4
1c009780:	41f686b3          	sub	a3,a3,t6
1c009784:	03e6d6b3          	divu	a3,a3,t5
1c009788:	8275                	srli	a2,a2,0x1d
1c00978a:	95b2                	add	a1,a1,a2
1c00978c:	9736                	add	a4,a4,a3
1c00978e:	00d736b3          	sltu	a3,a4,a3
1c009792:	95b6                	add	a1,a1,a3
1c009794:	01f75693          	srli	a3,a4,0x1f
1c009798:	00159613          	slli	a2,a1,0x1
1c00979c:	00c6e5b3          	or	a1,a3,a2
1c0097a0:	0706                	slli	a4,a4,0x1
1c0097a2:	157d                	addi	a0,a0,-1
1c0097a4:	febe78e3          	bleu	a1,t3,1c009794 <pos_libc_prf+0xd1a>
1c0097a8:	bf79                	j	1c009746 <pos_libc_prf+0xccc>
1c0097aa:	04c00613          	li	a2,76
1c0097ae:	004aa68b          	p.lw	a3,4(s5!)
1c0097b2:	4cc708e3          	beq	a4,a2,1c00a482 <pos_libc_prf+0x1a08>
1c0097b6:	16e65663          	ble	a4,a2,1c009922 <pos_libc_prf+0xea8>
1c0097ba:	06800613          	li	a2,104
1c0097be:	00c70563          	beq	a4,a2,1c0097c8 <pos_libc_prf+0xd4e>
1c0097c2:	c280                	sw	s0,0(a3)
1c0097c4:	af2ff06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c0097c8:	8722                	mv	a4,s0
1c0097ca:	00e69023          	sh	a4,0(a3)
1c0097ce:	ae8ff06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c0097d2:	004a8793          	addi	a5,s5,4
1c0097d6:	ce3e                	sw	a5,28(sp)
1c0097d8:	000aa703          	lw	a4,0(s5)
1c0097dc:	77e1                	lui	a5,0xffff8
1c0097de:	02610813          	addi	a6,sp,38
1c0097e2:	8307c793          	xori	a5,a5,-2000
1c0097e6:	4601                	li	a2,0
1c0097e8:	02f11223          	sh	a5,36(sp)
1c0097ec:	86c2                	mv	a3,a6
1c0097ee:	4ea5                	li	t4,9
1c0097f0:	f64735b3          	p.bclr	a1,a4,27,4
1c0097f4:	01c61513          	slli	a0,a2,0x1c
1c0097f8:	8311                	srli	a4,a4,0x4
1c0097fa:	8f49                	or	a4,a4,a0
1c0097fc:	8211                	srli	a2,a2,0x4
1c0097fe:	05758e13          	addi	t3,a1,87
1c009802:	03058893          	addi	a7,a1,48
1c009806:	00c76333          	or	t1,a4,a2
1c00980a:	00befa63          	bleu	a1,t4,1c00981e <pos_libc_prf+0xda4>
1c00980e:	01c68023          	sb	t3,0(a3)
1c009812:	00168593          	addi	a1,a3,1
1c009816:	00030a63          	beqz	t1,1c00982a <pos_libc_prf+0xdb0>
1c00981a:	86ae                	mv	a3,a1
1c00981c:	bfd1                	j	1c0097f0 <pos_libc_prf+0xd76>
1c00981e:	01168023          	sb	a7,0(a3)
1c009822:	00168593          	addi	a1,a3,1
1c009826:	fe031ae3          	bnez	t1,1c00981a <pos_libc_prf+0xda0>
1c00982a:	00058023          	sb	zero,0(a1)
1c00982e:	410585b3          	sub	a1,a1,a6
1c009832:	00d87e63          	bleu	a3,a6,1c00984e <pos_libc_prf+0xdd4>
1c009836:	8736                	mv	a4,a3
1c009838:	86c2                	mv	a3,a6
1c00983a:	0006c503          	lbu	a0,0(a3)
1c00983e:	00074603          	lbu	a2,0(a4)
1c009842:	fea70fab          	p.sb	a0,-1(a4!)
1c009846:	00c680ab          	p.sb	a2,1(a3!)
1c00984a:	fee6e8e3          	bltu	a3,a4,1c00983a <pos_libc_prf+0xdc0>
1c00984e:	4789                	li	a5,2
1c009850:	00258b93          	addi	s7,a1,2
1c009854:	c23e                	sw	a5,4(sp)
1c009856:	ca6ff06f          	j	1c008cfc <pos_libc_prf+0x282>
1c00985a:	85a6                	mv	a1,s1
1c00985c:	02500513          	li	a0,37
1c009860:	9902                	jalr	s2
1c009862:	aff52263          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c009866:	85a6                	mv	a1,s1
1c009868:	8562                	mv	a0,s8
1c00986a:	9902                	jalr	s2
1c00986c:	adf52d63          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c009870:	0409                	addi	s0,s0,2
1c009872:	a44ff06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c009876:	001ccc03          	lbu	s8,1(s9)
1c00987a:	04c00713          	li	a4,76
1c00987e:	0c89                	addi	s9,s9,2
1c009880:	b68ff06f          	j	1c008be8 <pos_libc_prf+0x16e>
1c009884:	001ccc03          	lbu	s8,1(s9)
1c009888:	04800713          	li	a4,72
1c00988c:	0c89                	addi	s9,s9,2
1c00988e:	b5aff06f          	j	1c008be8 <pos_libc_prf+0x16e>
1c009892:	4785                	li	a5,1
1c009894:	02514703          	lbu	a4,37(sp)
1c009898:	c23e                	sw	a5,4(sp)
1c00989a:	b385                	j	1c0095fa <pos_libc_prf+0xb80>
1c00989c:	4785                	li	a5,1
1c00989e:	c23e                	sw	a5,4(sp)
1c0098a0:	c5cff06f          	j	1c008cfc <pos_libc_prf+0x282>
1c0098a4:	004a8793          	addi	a5,s5,4
1c0098a8:	ce3e                	sw	a5,28(sp)
1c0098aa:	000aab83          	lw	s7,0(s5)
1c0098ae:	4d01                	li	s10,0
1c0098b0:	e64ff06f          	j	1c008f14 <pos_libc_prf+0x49a>
1c0098b4:	41bd0d33          	sub	s10,s10,s11
1c0098b8:	8bea                	mv	s7,s10
1c0098ba:	a039                	j	1c0098c8 <pos_libc_prf+0xe4e>
1c0098bc:	001c450b          	p.lbu	a0,1(s8!)
1c0098c0:	1bfd                	addi	s7,s7,-1
1c0098c2:	9902                	jalr	s2
1c0098c4:	a9f52163          	p.beqimm	a0,-1,1c008b46 <pos_libc_prf+0xcc>
1c0098c8:	85a6                	mv	a1,s1
1c0098ca:	ff7049e3          	bgtz	s7,1c0098bc <pos_libc_prf+0xe42>
1c0098ce:	4792                	lw	a5,4(sp)
1c0098d0:	4af2                	lw	s5,28(sp)
1c0098d2:	943e                	add	s0,s0,a5
1c0098d4:	47a2                	lw	a5,8(sp)
1c0098d6:	944e                	add	s0,s0,s3
1c0098d8:	943e                	add	s0,s0,a5
1c0098da:	9d605e63          	blez	s6,1c008ab6 <pos_libc_prf+0x3c>
1c0098de:	945a                	add	s0,s0,s6
1c0098e0:	1b7d                	addi	s6,s6,-1
1c0098e2:	a019                	j	1c0098e8 <pos_libc_prf+0xe6e>
1c0098e4:	3dfb2fe3          	p.beqimm	s6,-1,1c00a4c2 <pos_libc_prf+0x1a48>
1c0098e8:	85a6                	mv	a1,s1
1c0098ea:	02000513          	li	a0,32
1c0098ee:	9902                	jalr	s2
1c0098f0:	1b7d                	addi	s6,s6,-1
1c0098f2:	fff539e3          	p.bneimm	a0,-1,1c0098e4 <pos_libc_prf+0xe6a>
1c0098f6:	547d                	li	s0,-1
1c0098f8:	a50ff06f          	j	1c008b48 <pos_libc_prf+0xce>
1c0098fc:	47c2                	lw	a5,16(sp)
1c0098fe:	02410313          	addi	t1,sp,36
1c009902:	d60787e3          	beqz	a5,1c009670 <pos_libc_prf+0xbf6>
1c009906:	02000693          	li	a3,32
1c00990a:	02d10223          	sb	a3,36(sp)
1c00990e:	02510313          	addi	t1,sp,37
1c009912:	bbb9                	j	1c009670 <pos_libc_prf+0xbf6>
1c009914:	02d00693          	li	a3,45
1c009918:	02d10223          	sb	a3,36(sp)
1c00991c:	02510313          	addi	t1,sp,37
1c009920:	bb81                	j	1c009670 <pos_libc_prf+0xbf6>
1c009922:	04800613          	li	a2,72
1c009926:	e8c71ee3          	bne	a4,a2,1c0097c2 <pos_libc_prf+0xd48>
1c00992a:	8722                	mv	a4,s0
1c00992c:	00e68023          	sb	a4,0(a3)
1c009930:	986ff06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c009934:	40e00733          	neg	a4,a4
1c009938:	02510793          	addi	a5,sp,37
1c00993c:	00e036b3          	snez	a3,a4
1c009940:	41500db3          	neg	s11,s5
1c009944:	02d00613          	li	a2,45
1c009948:	02c10223          	sb	a2,36(sp)
1c00994c:	40dd8db3          	sub	s11,s11,a3
1c009950:	c23e                	sw	a5,4(sp)
1c009952:	8c3e                	mv	s8,a5
1c009954:	b38ff06f          	j	1c008c8c <pos_libc_prf+0x212>
1c009958:	8f4d                	or	a4,a4,a1
1c00995a:	fbfc0813          	addi	a6,s8,-65
1c00995e:	00330e13          	addi	t3,t1,3
1c009962:	1a071763          	bnez	a4,1c009b10 <pos_libc_prf+0x1096>
1c009966:	4765                	li	a4,25
1c009968:	370760e3          	bltu	a4,a6,1c00a4c8 <pos_libc_prf+0x1a4e>
1c00996c:	6715                	lui	a4,0x5
1c00996e:	e4970713          	addi	a4,a4,-439 # 4e49 <pos_soc_event_callback+0x4a09>
1c009972:	00e31023          	sh	a4,0(t1)
1c009976:	04600713          	li	a4,70
1c00997a:	00e30123          	sb	a4,2(t1)
1c00997e:	4782                	lw	a5,0(sp)
1c009980:	000301a3          	sb	zero,3(t1)
1c009984:	4b81                	li	s7,0
1c009986:	40fe0e33          	sub	t3,t3,a5
1c00998a:	4d81                	li	s11,0
1c00998c:	cc02                	sw	zero,24(sp)
1c00998e:	4a81                	li	s5,0
1c009990:	b981                	j	1c0095e0 <pos_libc_prf+0xb66>
1c009992:	800009b7          	lui	s3,0x80000
1c009996:	fff9c993          	not	s3,s3
1c00999a:	cb0ff06f          	j	1c008e4a <pos_libc_prf+0x3d0>
1c00999e:	86f2                	mv	a3,t3
1c0099a0:	0186812b          	p.sb	s8,2(a3!)
1c0099a4:	2a08c5e3          	bltz	a7,1c00a44e <pos_libc_prf+0x19d4>
1c0099a8:	02b00713          	li	a4,43
1c0099ac:	00ee00a3          	sb	a4,1(t3) # 80000001 <pulp__FC+0x80000002>
1c0099b0:	06300713          	li	a4,99
1c0099b4:	01175e63          	ble	a7,a4,1c0099d0 <pos_libc_prf+0xf56>
1c0099b8:	06400613          	li	a2,100
1c0099bc:	02c8c733          	div	a4,a7,a2
1c0099c0:	003e0693          	addi	a3,t3,3
1c0099c4:	02c8e8b3          	rem	a7,a7,a2
1c0099c8:	03070713          	addi	a4,a4,48
1c0099cc:	00ee0123          	sb	a4,2(t3)
1c0099d0:	4729                	li	a4,10
1c0099d2:	8e36                	mv	t3,a3
1c0099d4:	02e8c633          	div	a2,a7,a4
1c0099d8:	02e8e733          	rem	a4,a7,a4
1c0099dc:	03060613          	addi	a2,a2,48
1c0099e0:	00ce012b          	p.sb	a2,2(t3!)
1c0099e4:	03070713          	addi	a4,a4,48
1c0099e8:	00e680a3          	sb	a4,1(a3)
1c0099ec:	b6ed                	j	1c0095d6 <pos_libc_prf+0xb5c>
1c0099ee:	5775                	li	a4,-3
1c0099f0:	02e8cd63          	blt	a7,a4,1c009a2a <pos_libc_prf+0xfb0>
1c0099f4:	0319cb63          	blt	s3,a7,1c009a2a <pos_libc_prf+0xfb0>
1c0099f8:	4792                	lw	a5,4(sp)
1c0099fa:	411989b3          	sub	s3,s3,a7
1c0099fe:	4581                	li	a1,0
1c009a00:	e789                	bnez	a5,1c009a0a <pos_libc_prf+0xf90>
1c009a02:	013025b3          	sgtz	a1,s3
1c009a06:	0ff5f593          	andi	a1,a1,255
1c009a0a:	01388733          	add	a4,a7,s3
1c009a0e:	280748e3          	bltz	a4,1c00a49e <pos_libc_prf+0x1a24>
1c009a12:	47c1                	li	a5,16
1c009a14:	04f74e33          	p.min	t3,a4,a5
1c009a18:	1e7d                	addi	t3,t3,-1
1c009a1a:	06600c13          	li	s8,102
1c009a1e:	e6071a63          	bnez	a4,1c009092 <pos_libc_prf+0x618>
1c009a22:	08000fb7          	lui	t6,0x8000
1c009a26:	edcff06f          	j	1c009102 <pos_libc_prf+0x688>
1c009a2a:	ffec0813          	addi	a6,s8,-2
1c009a2e:	0ff87c13          	andi	s8,a6,255
1c009a32:	2a0986e3          	beqz	s3,1c00a4de <pos_libc_prf+0x1a64>
1c009a36:	4792                	lw	a5,4(sp)
1c009a38:	fff98713          	addi	a4,s3,-1 # 7fffffff <pulp__FC+0x80000000>
1c009a3c:	240799e3          	bnez	a5,1c00a48e <pos_libc_prf+0x1a14>
1c009a40:	00e025b3          	sgtz	a1,a4
1c009a44:	0ff5f593          	andi	a1,a1,255
1c009a48:	89ba                	mv	s3,a4
1c009a4a:	e3cff06f          	j	1c009086 <pos_libc_prf+0x60c>
1c009a4e:	8bee                	mv	s7,s11
1c009a50:	cc02                	sw	zero,24(sp)
1c009a52:	4a81                	li	s5,0
1c009a54:	be9d                	j	1c0095ca <pos_libc_prf+0xb50>
1c009a56:	4881                	li	a7,0
1c009a58:	1054                	addi	a3,sp,36
1c009a5a:	ce4ff06f          	j	1c008f3e <pos_libc_prf+0x4c4>
1c009a5e:	007a8693          	addi	a3,s5,7
1c009a62:	c406b6b3          	p.bclr	a3,a3,2,0
1c009a66:	00868793          	addi	a5,a3,8
1c009a6a:	ce3e                	sw	a5,28(sp)
1c009a6c:	4298                	lw	a4,0(a3)
1c009a6e:	0046aa83          	lw	s5,4(a3)
1c009a72:	9f6ff06f          	j	1c008c68 <pos_libc_prf+0x1ee>
1c009a76:	007a8713          	addi	a4,s5,7
1c009a7a:	c4073733          	p.bclr	a4,a4,2,0
1c009a7e:	00870793          	addi	a5,a4,8
1c009a82:	ce3e                	sw	a5,28(sp)
1c009a84:	00072b83          	lw	s7,0(a4)
1c009a88:	00472d03          	lw	s10,4(a4)
1c009a8c:	c88ff06f          	j	1c008f14 <pos_libc_prf+0x49a>
1c009a90:	4792                	lw	a5,4(sp)
1c009a92:	1a0788e3          	beqz	a5,1c00a442 <pos_libc_prf+0x19c8>
1c009a96:	03000713          	li	a4,48
1c009a9a:	02e10223          	sb	a4,36(sp)
1c009a9e:	01abe733          	or	a4,s7,s10
1c009aa2:	240707e3          	beqz	a4,1c00a4f0 <pos_libc_prf+0x1a76>
1c009aa6:	02510813          	addi	a6,sp,37
1c009aaa:	4585                	li	a1,1
1c009aac:	86c2                	mv	a3,a6
1c009aae:	8736                	mv	a4,a3
1c009ab0:	a011                	j	1c009ab4 <pos_libc_prf+0x103a>
1c009ab2:	8732                	mv	a4,a2
1c009ab4:	01dd1513          	slli	a0,s10,0x1d
1c009ab8:	f83bb633          	p.bclr	a2,s7,28,3
1c009abc:	003bdb93          	srli	s7,s7,0x3
1c009ac0:	03060613          	addi	a2,a2,48
1c009ac4:	01756bb3          	or	s7,a0,s7
1c009ac8:	003d5d13          	srli	s10,s10,0x3
1c009acc:	00c70023          	sb	a2,0(a4)
1c009ad0:	01abe533          	or	a0,s7,s10
1c009ad4:	00170613          	addi	a2,a4,1
1c009ad8:	fd69                	bnez	a0,1c009ab2 <pos_libc_prf+0x1038>
1c009ada:	00060023          	sb	zero,0(a2)
1c009ade:	41060633          	sub	a2,a2,a6
1c009ae2:	00e6fc63          	bleu	a4,a3,1c009afa <pos_libc_prf+0x1080>
1c009ae6:	0006c803          	lbu	a6,0(a3)
1c009aea:	00074503          	lbu	a0,0(a4)
1c009aee:	ff070fab          	p.sb	a6,-1(a4!)
1c009af2:	00a680ab          	p.sb	a0,1(a3!)
1c009af6:	fee6e8e3          	bltu	a3,a4,1c009ae6 <pos_libc_prf+0x106c>
1c009afa:	00b60bb3          	add	s7,a2,a1
1c009afe:	c202                	sw	zero,4(sp)
1c009b00:	9fcff06f          	j	1c008cfc <pos_libc_prf+0x282>
1c009b04:	105c                	addi	a5,sp,36
1c009b06:	8dd6                	mv	s11,s5
1c009b08:	c23e                	sw	a5,4(sp)
1c009b0a:	8c3e                	mv	s8,a5
1c009b0c:	980ff06f          	j	1c008c8c <pos_libc_prf+0x212>
1c009b10:	4765                	li	a4,25
1c009b12:	19076de3          	bltu	a4,a6,1c00a4ac <pos_libc_prf+0x1a32>
1c009b16:	6711                	lui	a4,0x4
1c009b18:	14e70713          	addi	a4,a4,334 # 414e <pos_soc_event_callback+0x3d0e>
1c009b1c:	00e31023          	sh	a4,0(t1)
1c009b20:	04e00713          	li	a4,78
1c009b24:	00e30123          	sb	a4,2(t1)
1c009b28:	bd99                	j	1c00997e <pos_libc_prf+0xf04>
1c009b2a:	1058                	addi	a4,sp,36
1c009b2c:	4565                	li	a0,25
1c009b2e:	0017468b          	p.lbu	a3,1(a4!)
1c009b32:	f9f68593          	addi	a1,a3,-97
1c009b36:	0ff5f593          	andi	a1,a1,255
1c009b3a:	fe068813          	addi	a6,a3,-32
1c009b3e:	00b56e63          	bltu	a0,a1,1c009b5a <pos_libc_prf+0x10e0>
1c009b42:	ff070fa3          	sb	a6,-1(a4)
1c009b46:	0017468b          	p.lbu	a3,1(a4!)
1c009b4a:	f9f68593          	addi	a1,a3,-97
1c009b4e:	0ff5f593          	andi	a1,a1,255
1c009b52:	fe068813          	addi	a6,a3,-32
1c009b56:	feb576e3          	bleu	a1,a0,1c009b42 <pos_libc_prf+0x10c8>
1c009b5a:	faf1                	bnez	a3,1c009b2e <pos_libc_prf+0x10b4>
1c009b5c:	ca6ff06f          	j	1c009002 <pos_libc_prf+0x588>
1c009b60:	4d22                	lw	s10,8(sp)
1c009b62:	8bd6                	mv	s7,s5
1c009b64:	a039                	j	1c009b72 <pos_libc_prf+0x10f8>
1c009b66:	9902                	jalr	s2
1c009b68:	1bfd                	addi	s7,s7,-1
1c009b6a:	01f53463          	p.bneimm	a0,-1,1c009b72 <pos_libc_prf+0x10f8>
1c009b6e:	fd9fe06f          	j	1c008b46 <pos_libc_prf+0xcc>
1c009b72:	85a6                	mv	a1,s1
1c009b74:	03000513          	li	a0,48
1c009b78:	ff7047e3          	bgtz	s7,1c009b66 <pos_libc_prf+0x10ec>
1c009b7c:	415d0d33          	sub	s10,s10,s5
1c009b80:	9fcff06f          	j	1c008d7c <pos_libc_prf+0x302>
1c009b84:	04600e93          	li	t4,70
1c009b88:	06600c13          	li	s8,102
1c009b8c:	bce5                	j	1c009684 <pos_libc_prf+0xc0a>
1c009b8e:	4981                	li	s3,0
1c009b90:	c202                	sw	zero,4(sp)
1c009b92:	4b81                	li	s7,0
1c009b94:	984ff06f          	j	1c008d18 <pos_libc_prf+0x29e>
1c009b98:	02e00713          	li	a4,46
1c009b9c:	00e300a3          	sb	a4,1(t1)
1c009ba0:	00230713          	addi	a4,t1,2
1c009ba4:	e3304863          	bgtz	s3,1c0091d4 <pos_libc_prf+0x75a>
1c009ba8:	8dce                	mv	s11,s3
1c009baa:	8e3a                	mv	t3,a4
1c009bac:	bad5                	j	1c0095a0 <pos_libc_prf+0xb26>
1c009bae:	15105ce3          	blez	a7,1c00a506 <pos_libc_prf+0x1a8c>
1c009bb2:	00271e93          	slli	t4,a4,0x2
1c009bb6:	01e75f93          	srli	t6,a4,0x1e
1c009bba:	00269f13          	slli	t5,a3,0x2
1c009bbe:	00ee8633          	add	a2,t4,a4
1c009bc2:	01efef33          	or	t5,t6,t5
1c009bc6:	9f36                	add	t5,t5,a3
1c009bc8:	01d636b3          	sltu	a3,a2,t4
1c009bcc:	96fa                	add	a3,a3,t5
1c009bce:	01f65713          	srli	a4,a2,0x1f
1c009bd2:	0686                	slli	a3,a3,0x1
1c009bd4:	8ed9                	or	a3,a3,a4
1c009bd6:	01c6d713          	srli	a4,a3,0x1c
1c009bda:	03070713          	addi	a4,a4,48
1c009bde:	10000eb7          	lui	t4,0x10000
1c009be2:	1efd                	addi	t4,t4,-1
1c009be4:	00e30023          	sb	a4,0(t1)
1c009be8:	01d6f6b3          	and	a3,a3,t4
1c009bec:	00161713          	slli	a4,a2,0x1
1c009bf0:	2418a9e3          	p.beqimm	a7,1,1c00a642 <pos_libc_prf+0x1bc8>
1c009bf4:	01e75f93          	srli	t6,a4,0x1e
1c009bf8:	00269f13          	slli	t5,a3,0x2
1c009bfc:	060e                	slli	a2,a2,0x3
1c009bfe:	00e60e33          	add	t3,a2,a4
1c009c02:	01efef33          	or	t5,t6,t5
1c009c06:	9f36                	add	t5,t5,a3
1c009c08:	00ce36b3          	sltu	a3,t3,a2
1c009c0c:	96fa                	add	a3,a3,t5
1c009c0e:	01fe5713          	srli	a4,t3,0x1f
1c009c12:	0686                	slli	a3,a3,0x1
1c009c14:	8ed9                	or	a3,a3,a4
1c009c16:	01c6d713          	srli	a4,a3,0x1c
1c009c1a:	03070713          	addi	a4,a4,48
1c009c1e:	00e300a3          	sb	a4,1(t1)
1c009c22:	01d6f6b3          	and	a3,a3,t4
1c009c26:	001e1713          	slli	a4,t3,0x1
1c009c2a:	2028a3e3          	p.beqimm	a7,2,1c00a630 <pos_libc_prf+0x1bb6>
1c009c2e:	01e75f93          	srli	t6,a4,0x1e
1c009c32:	00269f13          	slli	t5,a3,0x2
1c009c36:	0e0e                	slli	t3,t3,0x3
1c009c38:	00ee0633          	add	a2,t3,a4
1c009c3c:	01efef33          	or	t5,t6,t5
1c009c40:	9f36                	add	t5,t5,a3
1c009c42:	01c636b3          	sltu	a3,a2,t3
1c009c46:	96fa                	add	a3,a3,t5
1c009c48:	01f65713          	srli	a4,a2,0x1f
1c009c4c:	0686                	slli	a3,a3,0x1
1c009c4e:	8ed9                	or	a3,a3,a4
1c009c50:	01c6d713          	srli	a4,a3,0x1c
1c009c54:	03070713          	addi	a4,a4,48
1c009c58:	00e30123          	sb	a4,2(t1)
1c009c5c:	ffd88a93          	addi	s5,a7,-3
1c009c60:	00161713          	slli	a4,a2,0x1
1c009c64:	01d6f6b3          	and	a3,a3,t4
1c009c68:	1d5050e3          	blez	s5,1c00a628 <pos_libc_prf+0x1bae>
1c009c6c:	01e75f93          	srli	t6,a4,0x1e
1c009c70:	00269f13          	slli	t5,a3,0x2
1c009c74:	060e                	slli	a2,a2,0x3
1c009c76:	00e60e33          	add	t3,a2,a4
1c009c7a:	01efef33          	or	t5,t6,t5
1c009c7e:	9f36                	add	t5,t5,a3
1c009c80:	00ce36b3          	sltu	a3,t3,a2
1c009c84:	96fa                	add	a3,a3,t5
1c009c86:	01fe5713          	srli	a4,t3,0x1f
1c009c8a:	0686                	slli	a3,a3,0x1
1c009c8c:	8ed9                	or	a3,a3,a4
1c009c8e:	01c6d713          	srli	a4,a3,0x1c
1c009c92:	03070713          	addi	a4,a4,48
1c009c96:	00e301a3          	sb	a4,3(t1)
1c009c9a:	ffc88a93          	addi	s5,a7,-4
1c009c9e:	001e1713          	slli	a4,t3,0x1
1c009ca2:	01d6f6b3          	and	a3,a3,t4
1c009ca6:	17505de3          	blez	s5,1c00a620 <pos_libc_prf+0x1ba6>
1c009caa:	01e75f93          	srli	t6,a4,0x1e
1c009cae:	00269f13          	slli	t5,a3,0x2
1c009cb2:	0e0e                	slli	t3,t3,0x3
1c009cb4:	00ee0633          	add	a2,t3,a4
1c009cb8:	01efef33          	or	t5,t6,t5
1c009cbc:	9f36                	add	t5,t5,a3
1c009cbe:	01c636b3          	sltu	a3,a2,t3
1c009cc2:	96fa                	add	a3,a3,t5
1c009cc4:	01f65713          	srli	a4,a2,0x1f
1c009cc8:	0686                	slli	a3,a3,0x1
1c009cca:	8ed9                	or	a3,a3,a4
1c009ccc:	01c6d713          	srli	a4,a3,0x1c
1c009cd0:	03070713          	addi	a4,a4,48
1c009cd4:	00e30223          	sb	a4,4(t1)
1c009cd8:	ffb88a93          	addi	s5,a7,-5
1c009cdc:	00161713          	slli	a4,a2,0x1
1c009ce0:	01d6f6b3          	and	a3,a3,t4
1c009ce4:	13505ae3          	blez	s5,1c00a618 <pos_libc_prf+0x1b9e>
1c009ce8:	01e75f93          	srli	t6,a4,0x1e
1c009cec:	00269f13          	slli	t5,a3,0x2
1c009cf0:	060e                	slli	a2,a2,0x3
1c009cf2:	00e60e33          	add	t3,a2,a4
1c009cf6:	01efef33          	or	t5,t6,t5
1c009cfa:	9f36                	add	t5,t5,a3
1c009cfc:	00ce36b3          	sltu	a3,t3,a2
1c009d00:	96fa                	add	a3,a3,t5
1c009d02:	01fe5713          	srli	a4,t3,0x1f
1c009d06:	0686                	slli	a3,a3,0x1
1c009d08:	8ed9                	or	a3,a3,a4
1c009d0a:	01c6d713          	srli	a4,a3,0x1c
1c009d0e:	03070713          	addi	a4,a4,48
1c009d12:	00e302a3          	sb	a4,5(t1)
1c009d16:	ffa88a93          	addi	s5,a7,-6
1c009d1a:	001e1713          	slli	a4,t3,0x1
1c009d1e:	01d6f6b3          	and	a3,a3,t4
1c009d22:	0f5057e3          	blez	s5,1c00a610 <pos_libc_prf+0x1b96>
1c009d26:	01e75f93          	srli	t6,a4,0x1e
1c009d2a:	00269f13          	slli	t5,a3,0x2
1c009d2e:	0e0e                	slli	t3,t3,0x3
1c009d30:	00ee0633          	add	a2,t3,a4
1c009d34:	01efef33          	or	t5,t6,t5
1c009d38:	9f36                	add	t5,t5,a3
1c009d3a:	01c636b3          	sltu	a3,a2,t3
1c009d3e:	96fa                	add	a3,a3,t5
1c009d40:	01f65713          	srli	a4,a2,0x1f
1c009d44:	0686                	slli	a3,a3,0x1
1c009d46:	8ed9                	or	a3,a3,a4
1c009d48:	01c6d713          	srli	a4,a3,0x1c
1c009d4c:	03070713          	addi	a4,a4,48
1c009d50:	00e30323          	sb	a4,6(t1)
1c009d54:	ff988a93          	addi	s5,a7,-7
1c009d58:	00161713          	slli	a4,a2,0x1
1c009d5c:	01d6f6b3          	and	a3,a3,t4
1c009d60:	0b5054e3          	blez	s5,1c00a608 <pos_libc_prf+0x1b8e>
1c009d64:	060e                	slli	a2,a2,0x3
1c009d66:	01e75f13          	srli	t5,a4,0x1e
1c009d6a:	00269e13          	slli	t3,a3,0x2
1c009d6e:	9732                	add	a4,a4,a2
1c009d70:	01cf6e33          	or	t3,t5,t3
1c009d74:	9e36                	add	t3,t3,a3
1c009d76:	00c736b3          	sltu	a3,a4,a2
1c009d7a:	96f2                	add	a3,a3,t3
1c009d7c:	01f75613          	srli	a2,a4,0x1f
1c009d80:	0686                	slli	a3,a3,0x1
1c009d82:	8ed1                	or	a3,a3,a2
1c009d84:	01c6d613          	srli	a2,a3,0x1c
1c009d88:	03060613          	addi	a2,a2,48
1c009d8c:	0706                	slli	a4,a4,0x1
1c009d8e:	00c303a3          	sb	a2,7(t1)
1c009d92:	ff888a93          	addi	s5,a7,-8
1c009d96:	863a                	mv	a2,a4
1c009d98:	01d6f6b3          	and	a3,a3,t4
1c009d9c:	075052e3          	blez	s5,1c00a600 <pos_libc_prf+0x1b86>
1c009da0:	070a                	slli	a4,a4,0x2
1c009da2:	01e65e93          	srli	t4,a2,0x1e
1c009da6:	00269e13          	slli	t3,a3,0x2
1c009daa:	963a                	add	a2,a2,a4
1c009dac:	01ceee33          	or	t3,t4,t3
1c009db0:	9e36                	add	t3,t3,a3
1c009db2:	00e636b3          	sltu	a3,a2,a4
1c009db6:	96f2                	add	a3,a3,t3
1c009db8:	01f65713          	srli	a4,a2,0x1f
1c009dbc:	0686                	slli	a3,a3,0x1
1c009dbe:	8ed9                	or	a3,a3,a4
1c009dc0:	01c6d713          	srli	a4,a3,0x1c
1c009dc4:	03070713          	addi	a4,a4,48
1c009dc8:	10000eb7          	lui	t4,0x10000
1c009dcc:	1efd                	addi	t4,t4,-1
1c009dce:	00e30423          	sb	a4,8(t1)
1c009dd2:	ff788a93          	addi	s5,a7,-9
1c009dd6:	00161713          	slli	a4,a2,0x1
1c009dda:	01d6f6b3          	and	a3,a3,t4
1c009dde:	01505de3          	blez	s5,1c00a5f8 <pos_libc_prf+0x1b7e>
1c009de2:	01e75f93          	srli	t6,a4,0x1e
1c009de6:	00269f13          	slli	t5,a3,0x2
1c009dea:	060e                	slli	a2,a2,0x3
1c009dec:	00e60e33          	add	t3,a2,a4
1c009df0:	01efef33          	or	t5,t6,t5
1c009df4:	9f36                	add	t5,t5,a3
1c009df6:	00ce36b3          	sltu	a3,t3,a2
1c009dfa:	96fa                	add	a3,a3,t5
1c009dfc:	01fe5713          	srli	a4,t3,0x1f
1c009e00:	0686                	slli	a3,a3,0x1
1c009e02:	8ed9                	or	a3,a3,a4
1c009e04:	01c6d713          	srli	a4,a3,0x1c
1c009e08:	03070713          	addi	a4,a4,48
1c009e0c:	00e304a3          	sb	a4,9(t1)
1c009e10:	ff688a93          	addi	s5,a7,-10
1c009e14:	001e1713          	slli	a4,t3,0x1
1c009e18:	01d6f6b3          	and	a3,a3,t4
1c009e1c:	7d505a63          	blez	s5,1c00a5f0 <pos_libc_prf+0x1b76>
1c009e20:	01e75f93          	srli	t6,a4,0x1e
1c009e24:	00269f13          	slli	t5,a3,0x2
1c009e28:	0e0e                	slli	t3,t3,0x3
1c009e2a:	00ee0633          	add	a2,t3,a4
1c009e2e:	01efef33          	or	t5,t6,t5
1c009e32:	9f36                	add	t5,t5,a3
1c009e34:	01c636b3          	sltu	a3,a2,t3
1c009e38:	96fa                	add	a3,a3,t5
1c009e3a:	01f65713          	srli	a4,a2,0x1f
1c009e3e:	0686                	slli	a3,a3,0x1
1c009e40:	8ed9                	or	a3,a3,a4
1c009e42:	01c6d713          	srli	a4,a3,0x1c
1c009e46:	03070713          	addi	a4,a4,48
1c009e4a:	00e30523          	sb	a4,10(t1)
1c009e4e:	ff588a93          	addi	s5,a7,-11
1c009e52:	00161713          	slli	a4,a2,0x1
1c009e56:	01d6f6b3          	and	a3,a3,t4
1c009e5a:	79505763          	blez	s5,1c00a5e8 <pos_libc_prf+0x1b6e>
1c009e5e:	01e75f93          	srli	t6,a4,0x1e
1c009e62:	00269f13          	slli	t5,a3,0x2
1c009e66:	060e                	slli	a2,a2,0x3
1c009e68:	00e60e33          	add	t3,a2,a4
1c009e6c:	01efef33          	or	t5,t6,t5
1c009e70:	9f36                	add	t5,t5,a3
1c009e72:	00ce36b3          	sltu	a3,t3,a2
1c009e76:	96fa                	add	a3,a3,t5
1c009e78:	01fe5713          	srli	a4,t3,0x1f
1c009e7c:	0686                	slli	a3,a3,0x1
1c009e7e:	8ed9                	or	a3,a3,a4
1c009e80:	01c6d713          	srli	a4,a3,0x1c
1c009e84:	03070713          	addi	a4,a4,48
1c009e88:	00e305a3          	sb	a4,11(t1)
1c009e8c:	ff488a93          	addi	s5,a7,-12
1c009e90:	001e1713          	slli	a4,t3,0x1
1c009e94:	01d6f6b3          	and	a3,a3,t4
1c009e98:	75505563          	blez	s5,1c00a5e2 <pos_libc_prf+0x1b68>
1c009e9c:	003e1513          	slli	a0,t3,0x3
1c009ea0:	01e75f93          	srli	t6,a4,0x1e
1c009ea4:	00269f13          	slli	t5,a3,0x2
1c009ea8:	00e50633          	add	a2,a0,a4
1c009eac:	01efef33          	or	t5,t6,t5
1c009eb0:	9f36                	add	t5,t5,a3
1c009eb2:	00a636b3          	sltu	a3,a2,a0
1c009eb6:	96fa                	add	a3,a3,t5
1c009eb8:	01f65713          	srli	a4,a2,0x1f
1c009ebc:	0686                	slli	a3,a3,0x1
1c009ebe:	8ed9                	or	a3,a3,a4
1c009ec0:	01c6d713          	srli	a4,a3,0x1c
1c009ec4:	03070713          	addi	a4,a4,48
1c009ec8:	00e30623          	sb	a4,12(t1)
1c009ecc:	ff388a93          	addi	s5,a7,-13
1c009ed0:	00161713          	slli	a4,a2,0x1
1c009ed4:	01d6f6b3          	and	a3,a3,t4
1c009ed8:	00d30e13          	addi	t3,t1,13
1c009edc:	450d                	li	a0,3
1c009ede:	0d505163          	blez	s5,1c009fa0 <pos_libc_prf+0x1526>
1c009ee2:	01e75f13          	srli	t5,a4,0x1e
1c009ee6:	00269e13          	slli	t3,a3,0x2
1c009eea:	060e                	slli	a2,a2,0x3
1c009eec:	00e60533          	add	a0,a2,a4
1c009ef0:	01cf6e33          	or	t3,t5,t3
1c009ef4:	9e36                	add	t3,t3,a3
1c009ef6:	00c536b3          	sltu	a3,a0,a2
1c009efa:	96f2                	add	a3,a3,t3
1c009efc:	01f55713          	srli	a4,a0,0x1f
1c009f00:	0686                	slli	a3,a3,0x1
1c009f02:	8ed9                	or	a3,a3,a4
1c009f04:	01c6d713          	srli	a4,a3,0x1c
1c009f08:	03070713          	addi	a4,a4,48
1c009f0c:	00e306a3          	sb	a4,13(t1)
1c009f10:	ff288a93          	addi	s5,a7,-14
1c009f14:	00151713          	slli	a4,a0,0x1
1c009f18:	01d6f6b3          	and	a3,a3,t4
1c009f1c:	71505f63          	blez	s5,1c00a63a <pos_libc_prf+0x1bc0>
1c009f20:	050e                	slli	a0,a0,0x3
1c009f22:	01e75f13          	srli	t5,a4,0x1e
1c009f26:	00269e13          	slli	t3,a3,0x2
1c009f2a:	00e50633          	add	a2,a0,a4
1c009f2e:	01cf6e33          	or	t3,t5,t3
1c009f32:	9e36                	add	t3,t3,a3
1c009f34:	00a636b3          	sltu	a3,a2,a0
1c009f38:	96f2                	add	a3,a3,t3
1c009f3a:	01f65713          	srli	a4,a2,0x1f
1c009f3e:	0686                	slli	a3,a3,0x1
1c009f40:	8ed9                	or	a3,a3,a4
1c009f42:	01c6d713          	srli	a4,a3,0x1c
1c009f46:	03070713          	addi	a4,a4,48
1c009f4a:	00e30723          	sb	a4,14(t1)
1c009f4e:	ff188a93          	addi	s5,a7,-15
1c009f52:	00161713          	slli	a4,a2,0x1
1c009f56:	01d6f6b3          	and	a3,a3,t4
1c009f5a:	00f30e13          	addi	t3,t1,15
1c009f5e:	4505                	li	a0,1
1c009f60:	05505063          	blez	s5,1c009fa0 <pos_libc_prf+0x1526>
1c009f64:	01e75e13          	srli	t3,a4,0x1e
1c009f68:	060e                	slli	a2,a2,0x3
1c009f6a:	00269513          	slli	a0,a3,0x2
1c009f6e:	9732                	add	a4,a4,a2
1c009f70:	00ae6533          	or	a0,t3,a0
1c009f74:	9536                	add	a0,a0,a3
1c009f76:	00c736b3          	sltu	a3,a4,a2
1c009f7a:	96aa                	add	a3,a3,a0
1c009f7c:	01f75613          	srli	a2,a4,0x1f
1c009f80:	0686                	slli	a3,a3,0x1
1c009f82:	8ed1                	or	a3,a3,a2
1c009f84:	01c6d613          	srli	a2,a3,0x1c
1c009f88:	03060613          	addi	a2,a2,48
1c009f8c:	01030e13          	addi	t3,t1,16
1c009f90:	0706                	slli	a4,a4,0x1
1c009f92:	01d6f6b3          	and	a3,a3,t4
1c009f96:	00c307a3          	sb	a2,15(t1)
1c009f9a:	ff088a93          	addi	s5,a7,-16
1c009f9e:	4501                	li	a0,0
1c009fa0:	4792                	lw	a5,4(sp)
1c009fa2:	54078d63          	beqz	a5,1c00a4fc <pos_libc_prf+0x1a82>
1c009fa6:	02e00893          	li	a7,46
1c009faa:	011e0023          	sb	a7,0(t3)
1c009fae:	834e                	mv	t1,s3
1c009fb0:	cc02                	sw	zero,24(sp)
1c009fb2:	001e0613          	addi	a2,t3,1
1c009fb6:	4881                	li	a7,0
1c009fb8:	5e605263          	blez	t1,1c00a59c <pos_libc_prf+0x1b22>
1c009fbc:	5e050063          	beqz	a0,1c00a59c <pos_libc_prf+0x1b22>
1c009fc0:	01e75f13          	srli	t5,a4,0x1e
1c009fc4:	00271e13          	slli	t3,a4,0x2
1c009fc8:	00269e93          	slli	t4,a3,0x2
1c009fcc:	9772                	add	a4,a4,t3
1c009fce:	01df6eb3          	or	t4,t5,t4
1c009fd2:	01c73e33          	sltu	t3,a4,t3
1c009fd6:	96f6                	add	a3,a3,t4
1c009fd8:	96f2                	add	a3,a3,t3
1c009fda:	0686                	slli	a3,a3,0x1
1c009fdc:	01f75e13          	srli	t3,a4,0x1f
1c009fe0:	00de66b3          	or	a3,t3,a3
1c009fe4:	01c6de13          	srli	t3,a3,0x1c
1c009fe8:	030e0e13          	addi	t3,t3,48
1c009fec:	01c60023          	sb	t3,0(a2)
1c009ff0:	fff30993          	addi	s3,t1,-1
1c009ff4:	00160e13          	addi	t3,a2,1
1c009ff8:	00171f13          	slli	t5,a4,0x1
1c009ffc:	42098a63          	beqz	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a000:	42152863          	p.beqimm	a0,1,1c00a430 <pos_libc_prf+0x19b6>
1c00a004:	10000eb7          	lui	t4,0x10000
1c00a008:	1efd                	addi	t4,t4,-1
1c00a00a:	01d6f6b3          	and	a3,a3,t4
1c00a00e:	01ef5f93          	srli	t6,t5,0x1e
1c00a012:	070e                	slli	a4,a4,0x3
1c00a014:	00269e13          	slli	t3,a3,0x2
1c00a018:	9f3a                	add	t5,t5,a4
1c00a01a:	01cfee33          	or	t3,t6,t3
1c00a01e:	96f2                	add	a3,a3,t3
1c00a020:	00ef3733          	sltu	a4,t5,a4
1c00a024:	9736                	add	a4,a4,a3
1c00a026:	0706                	slli	a4,a4,0x1
1c00a028:	01ff5693          	srli	a3,t5,0x1f
1c00a02c:	8f55                	or	a4,a4,a3
1c00a02e:	01c75693          	srli	a3,a4,0x1c
1c00a032:	03068693          	addi	a3,a3,48
1c00a036:	00d600a3          	sb	a3,1(a2)
1c00a03a:	ffe30993          	addi	s3,t1,-2
1c00a03e:	00260e13          	addi	t3,a2,2
1c00a042:	001f1293          	slli	t0,t5,0x1
1c00a046:	01d77fb3          	and	t6,a4,t4
1c00a04a:	3e098363          	beqz	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a04e:	3e252163          	p.beqimm	a0,2,1c00a430 <pos_libc_prf+0x19b6>
1c00a052:	003f1693          	slli	a3,t5,0x3
1c00a056:	002f9e13          	slli	t3,t6,0x2
1c00a05a:	01e2df13          	srli	t5,t0,0x1e
1c00a05e:	00568733          	add	a4,a3,t0
1c00a062:	01cf6e33          	or	t3,t5,t3
1c00a066:	9e7e                	add	t3,t3,t6
1c00a068:	00d736b3          	sltu	a3,a4,a3
1c00a06c:	96f2                	add	a3,a3,t3
1c00a06e:	0686                	slli	a3,a3,0x1
1c00a070:	01f75e13          	srli	t3,a4,0x1f
1c00a074:	00de66b3          	or	a3,t3,a3
1c00a078:	01c6de13          	srli	t3,a3,0x1c
1c00a07c:	030e0e13          	addi	t3,t3,48
1c00a080:	01c60123          	sb	t3,2(a2)
1c00a084:	ffd30993          	addi	s3,t1,-3
1c00a088:	00360e13          	addi	t3,a2,3
1c00a08c:	00171f93          	slli	t6,a4,0x1
1c00a090:	01d6ff33          	and	t5,a3,t4
1c00a094:	39305e63          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a098:	38352c63          	p.beqimm	a0,3,1c00a430 <pos_libc_prf+0x19b6>
1c00a09c:	070e                	slli	a4,a4,0x3
1c00a09e:	002f1e13          	slli	t3,t5,0x2
1c00a0a2:	01efd293          	srli	t0,t6,0x1e
1c00a0a6:	01f706b3          	add	a3,a4,t6
1c00a0aa:	01c2ee33          	or	t3,t0,t3
1c00a0ae:	9e7a                	add	t3,t3,t5
1c00a0b0:	00e6b733          	sltu	a4,a3,a4
1c00a0b4:	9772                	add	a4,a4,t3
1c00a0b6:	0706                	slli	a4,a4,0x1
1c00a0b8:	01f6de13          	srli	t3,a3,0x1f
1c00a0bc:	00ee6733          	or	a4,t3,a4
1c00a0c0:	01c75e13          	srli	t3,a4,0x1c
1c00a0c4:	030e0e13          	addi	t3,t3,48
1c00a0c8:	01c601a3          	sb	t3,3(a2)
1c00a0cc:	ffc30993          	addi	s3,t1,-4
1c00a0d0:	00460e13          	addi	t3,a2,4
1c00a0d4:	00169f13          	slli	t5,a3,0x1
1c00a0d8:	01d77733          	and	a4,a4,t4
1c00a0dc:	35305a63          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a0e0:	34452863          	p.beqimm	a0,4,1c00a430 <pos_libc_prf+0x19b6>
1c00a0e4:	068e                	slli	a3,a3,0x3
1c00a0e6:	01ef5f93          	srli	t6,t5,0x1e
1c00a0ea:	00271e13          	slli	t3,a4,0x2
1c00a0ee:	01e68eb3          	add	t4,a3,t5
1c00a0f2:	01cfee33          	or	t3,t6,t3
1c00a0f6:	00deb6b3          	sltu	a3,t4,a3
1c00a0fa:	9772                	add	a4,a4,t3
1c00a0fc:	9736                	add	a4,a4,a3
1c00a0fe:	0706                	slli	a4,a4,0x1
1c00a100:	01fed693          	srli	a3,t4,0x1f
1c00a104:	8f55                	or	a4,a4,a3
1c00a106:	01c75693          	srli	a3,a4,0x1c
1c00a10a:	03068693          	addi	a3,a3,48
1c00a10e:	00d60223          	sb	a3,4(a2)
1c00a112:	ffb30993          	addi	s3,t1,-5
1c00a116:	00560e13          	addi	t3,a2,5
1c00a11a:	001e9f13          	slli	t5,t4,0x1
1c00a11e:	31305963          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a122:	30552763          	p.beqimm	a0,5,1c00a430 <pos_libc_prf+0x19b6>
1c00a126:	100006b7          	lui	a3,0x10000
1c00a12a:	16fd                	addi	a3,a3,-1
1c00a12c:	00d77fb3          	and	t6,a4,a3
1c00a130:	002f1e93          	slli	t4,t5,0x2
1c00a134:	01ef5713          	srli	a4,t5,0x1e
1c00a138:	002f9293          	slli	t0,t6,0x2
1c00a13c:	9f76                	add	t5,t5,t4
1c00a13e:	005762b3          	or	t0,a4,t0
1c00a142:	9f96                	add	t6,t6,t0
1c00a144:	01df3733          	sltu	a4,t5,t4
1c00a148:	977e                	add	a4,a4,t6
1c00a14a:	01ff5e13          	srli	t3,t5,0x1f
1c00a14e:	0706                	slli	a4,a4,0x1
1c00a150:	00ee6733          	or	a4,t3,a4
1c00a154:	01c75e13          	srli	t3,a4,0x1c
1c00a158:	030e0e13          	addi	t3,t3,48
1c00a15c:	01c602a3          	sb	t3,5(a2)
1c00a160:	ffa30993          	addi	s3,t1,-6
1c00a164:	00660e13          	addi	t3,a2,6
1c00a168:	001f1e93          	slli	t4,t5,0x1
1c00a16c:	00d77fb3          	and	t6,a4,a3
1c00a170:	2d305063          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a174:	2a652e63          	p.beqimm	a0,6,1c00a430 <pos_libc_prf+0x19b6>
1c00a178:	003f1e13          	slli	t3,t5,0x3
1c00a17c:	01eed293          	srli	t0,t4,0x1e
1c00a180:	002f9f13          	slli	t5,t6,0x2
1c00a184:	01de0733          	add	a4,t3,t4
1c00a188:	01e2ef33          	or	t5,t0,t5
1c00a18c:	9f7e                	add	t5,t5,t6
1c00a18e:	01c73eb3          	sltu	t4,a4,t3
1c00a192:	9efa                	add	t4,t4,t5
1c00a194:	01f75e13          	srli	t3,a4,0x1f
1c00a198:	0e86                	slli	t4,t4,0x1
1c00a19a:	01de6eb3          	or	t4,t3,t4
1c00a19e:	01cede13          	srli	t3,t4,0x1c
1c00a1a2:	030e0e13          	addi	t3,t3,48
1c00a1a6:	01c60323          	sb	t3,6(a2)
1c00a1aa:	ff930993          	addi	s3,t1,-7
1c00a1ae:	00760e13          	addi	t3,a2,7
1c00a1b2:	00171f93          	slli	t6,a4,0x1
1c00a1b6:	00deff33          	and	t5,t4,a3
1c00a1ba:	27305b63          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a1be:	26752963          	p.beqimm	a0,7,1c00a430 <pos_libc_prf+0x19b6>
1c00a1c2:	070e                	slli	a4,a4,0x3
1c00a1c4:	01efde13          	srli	t3,t6,0x1e
1c00a1c8:	002f1293          	slli	t0,t5,0x2
1c00a1cc:	9fba                	add	t6,t6,a4
1c00a1ce:	005e62b3          	or	t0,t3,t0
1c00a1d2:	00efbeb3          	sltu	t4,t6,a4
1c00a1d6:	9f16                	add	t5,t5,t0
1c00a1d8:	9efa                	add	t4,t4,t5
1c00a1da:	01ffd713          	srli	a4,t6,0x1f
1c00a1de:	0e86                	slli	t4,t4,0x1
1c00a1e0:	01d76eb3          	or	t4,a4,t4
1c00a1e4:	01ced713          	srli	a4,t4,0x1c
1c00a1e8:	03070713          	addi	a4,a4,48
1c00a1ec:	00e603a3          	sb	a4,7(a2)
1c00a1f0:	ff830993          	addi	s3,t1,-8
1c00a1f4:	00860e13          	addi	t3,a2,8
1c00a1f8:	001f9713          	slli	a4,t6,0x1
1c00a1fc:	00deff33          	and	t5,t4,a3
1c00a200:	23305863          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a204:	22852663          	p.beqimm	a0,8,1c00a430 <pos_libc_prf+0x19b6>
1c00a208:	003f9e13          	slli	t3,t6,0x3
1c00a20c:	01e75e93          	srli	t4,a4,0x1e
1c00a210:	002f1f93          	slli	t6,t5,0x2
1c00a214:	9772                	add	a4,a4,t3
1c00a216:	01feefb3          	or	t6,t4,t6
1c00a21a:	9f7e                	add	t5,t5,t6
1c00a21c:	01c73eb3          	sltu	t4,a4,t3
1c00a220:	9efa                	add	t4,t4,t5
1c00a222:	01f75e13          	srli	t3,a4,0x1f
1c00a226:	0e86                	slli	t4,t4,0x1
1c00a228:	01de6eb3          	or	t4,t3,t4
1c00a22c:	01cede13          	srli	t3,t4,0x1c
1c00a230:	030e0e13          	addi	t3,t3,48
1c00a234:	01c60423          	sb	t3,8(a2)
1c00a238:	ff730993          	addi	s3,t1,-9
1c00a23c:	00960e13          	addi	t3,a2,9
1c00a240:	00171f13          	slli	t5,a4,0x1
1c00a244:	00defeb3          	and	t4,t4,a3
1c00a248:	1f305463          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a24c:	1e952263          	p.beqimm	a0,9,1c00a430 <pos_libc_prf+0x19b6>
1c00a250:	070e                	slli	a4,a4,0x3
1c00a252:	01ef5f93          	srli	t6,t5,0x1e
1c00a256:	002e9e13          	slli	t3,t4,0x2
1c00a25a:	9f3a                	add	t5,t5,a4
1c00a25c:	01cfee33          	or	t3,t6,t3
1c00a260:	9ef2                	add	t4,t4,t3
1c00a262:	00ef3733          	sltu	a4,t5,a4
1c00a266:	9776                	add	a4,a4,t4
1c00a268:	01ff5e13          	srli	t3,t5,0x1f
1c00a26c:	0706                	slli	a4,a4,0x1
1c00a26e:	00ee6733          	or	a4,t3,a4
1c00a272:	01c75e13          	srli	t3,a4,0x1c
1c00a276:	030e0e13          	addi	t3,t3,48
1c00a27a:	01c604a3          	sb	t3,9(a2)
1c00a27e:	ff630993          	addi	s3,t1,-10
1c00a282:	00a60e13          	addi	t3,a2,10
1c00a286:	001f1e93          	slli	t4,t5,0x1
1c00a28a:	8f75                	and	a4,a4,a3
1c00a28c:	1b305263          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a290:	1aa52063          	p.beqimm	a0,10,1c00a430 <pos_libc_prf+0x19b6>
1c00a294:	002e9e13          	slli	t3,t4,0x2
1c00a298:	01eedf93          	srli	t6,t4,0x1e
1c00a29c:	00271f13          	slli	t5,a4,0x2
1c00a2a0:	01de06b3          	add	a3,t3,t4
1c00a2a4:	01efef33          	or	t5,t6,t5
1c00a2a8:	01c6be33          	sltu	t3,a3,t3
1c00a2ac:	977a                	add	a4,a4,t5
1c00a2ae:	9772                	add	a4,a4,t3
1c00a2b0:	00171f13          	slli	t5,a4,0x1
1c00a2b4:	01f6de13          	srli	t3,a3,0x1f
1c00a2b8:	01ee6f33          	or	t5,t3,t5
1c00a2bc:	01cf5e13          	srli	t3,t5,0x1c
1c00a2c0:	030e0e13          	addi	t3,t3,48
1c00a2c4:	10000737          	lui	a4,0x10000
1c00a2c8:	fff70e93          	addi	t4,a4,-1 # fffffff <__l1_heap_size+0xfff0003>
1c00a2cc:	01c60523          	sb	t3,10(a2)
1c00a2d0:	ff530993          	addi	s3,t1,-11
1c00a2d4:	00b60e13          	addi	t3,a2,11
1c00a2d8:	00169713          	slli	a4,a3,0x1
1c00a2dc:	01df7f33          	and	t5,t5,t4
1c00a2e0:	15305863          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a2e4:	14b52663          	p.beqimm	a0,11,1c00a430 <pos_libc_prf+0x19b6>
1c00a2e8:	068e                	slli	a3,a3,0x3
1c00a2ea:	01e75f93          	srli	t6,a4,0x1e
1c00a2ee:	002f1e13          	slli	t3,t5,0x2
1c00a2f2:	9736                	add	a4,a4,a3
1c00a2f4:	01cfee33          	or	t3,t6,t3
1c00a2f8:	9f72                	add	t5,t5,t3
1c00a2fa:	00d736b3          	sltu	a3,a4,a3
1c00a2fe:	96fa                	add	a3,a3,t5
1c00a300:	01f75e13          	srli	t3,a4,0x1f
1c00a304:	0686                	slli	a3,a3,0x1
1c00a306:	00de66b3          	or	a3,t3,a3
1c00a30a:	01c6de13          	srli	t3,a3,0x1c
1c00a30e:	030e0e13          	addi	t3,t3,48
1c00a312:	01c605a3          	sb	t3,11(a2)
1c00a316:	ff430993          	addi	s3,t1,-12
1c00a31a:	00c60e13          	addi	t3,a2,12
1c00a31e:	00171f13          	slli	t5,a4,0x1
1c00a322:	11305763          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a326:	10c52563          	p.beqimm	a0,12,1c00a430 <pos_libc_prf+0x19b6>
1c00a32a:	01d6fe33          	and	t3,a3,t4
1c00a32e:	070e                	slli	a4,a4,0x3
1c00a330:	01ef5293          	srli	t0,t5,0x1e
1c00a334:	002e1f93          	slli	t6,t3,0x2
1c00a338:	01e706b3          	add	a3,a4,t5
1c00a33c:	01f2efb3          	or	t6,t0,t6
1c00a340:	00e6bf33          	sltu	t5,a3,a4
1c00a344:	9e7e                	add	t3,t3,t6
1c00a346:	9f72                	add	t5,t5,t3
1c00a348:	01f6d713          	srli	a4,a3,0x1f
1c00a34c:	0f06                	slli	t5,t5,0x1
1c00a34e:	01e76f33          	or	t5,a4,t5
1c00a352:	01cf5713          	srli	a4,t5,0x1c
1c00a356:	03070713          	addi	a4,a4,48
1c00a35a:	00e60623          	sb	a4,12(a2)
1c00a35e:	ff330993          	addi	s3,t1,-13
1c00a362:	00d60e13          	addi	t3,a2,13
1c00a366:	00169713          	slli	a4,a3,0x1
1c00a36a:	01df7f33          	and	t5,t5,t4
1c00a36e:	0d305163          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a372:	0ad52f63          	p.beqimm	a0,13,1c00a430 <pos_libc_prf+0x19b6>
1c00a376:	068e                	slli	a3,a3,0x3
1c00a378:	01e75f93          	srli	t6,a4,0x1e
1c00a37c:	002f1e13          	slli	t3,t5,0x2
1c00a380:	9736                	add	a4,a4,a3
1c00a382:	01cfee33          	or	t3,t6,t3
1c00a386:	9f72                	add	t5,t5,t3
1c00a388:	00d736b3          	sltu	a3,a4,a3
1c00a38c:	96fa                	add	a3,a3,t5
1c00a38e:	01f75e13          	srli	t3,a4,0x1f
1c00a392:	0686                	slli	a3,a3,0x1
1c00a394:	00de66b3          	or	a3,t3,a3
1c00a398:	01c6de13          	srli	t3,a3,0x1c
1c00a39c:	030e0e13          	addi	t3,t3,48
1c00a3a0:	01c606a3          	sb	t3,13(a2)
1c00a3a4:	ff230993          	addi	s3,t1,-14
1c00a3a8:	00e60e13          	addi	t3,a2,14
1c00a3ac:	00171f13          	slli	t5,a4,0x1
1c00a3b0:	01d6f6b3          	and	a3,a3,t4
1c00a3b4:	07305e63          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a3b8:	06e52c63          	p.beqimm	a0,14,1c00a430 <pos_libc_prf+0x19b6>
1c00a3bc:	070e                	slli	a4,a4,0x3
1c00a3be:	01ef5f93          	srli	t6,t5,0x1e
1c00a3c2:	00269e13          	slli	t3,a3,0x2
1c00a3c6:	9f3a                	add	t5,t5,a4
1c00a3c8:	01cfee33          	or	t3,t6,t3
1c00a3cc:	96f2                	add	a3,a3,t3
1c00a3ce:	00ef3733          	sltu	a4,t5,a4
1c00a3d2:	9736                	add	a4,a4,a3
1c00a3d4:	0706                	slli	a4,a4,0x1
1c00a3d6:	01ff5693          	srli	a3,t5,0x1f
1c00a3da:	8f55                	or	a4,a4,a3
1c00a3dc:	01c75693          	srli	a3,a4,0x1c
1c00a3e0:	03068693          	addi	a3,a3,48 # 10000030 <__l1_end+0x2c>
1c00a3e4:	00d60723          	sb	a3,14(a2)
1c00a3e8:	ff130993          	addi	s3,t1,-15
1c00a3ec:	00f60e13          	addi	t3,a2,15
1c00a3f0:	001f1693          	slli	a3,t5,0x1
1c00a3f4:	01d77733          	and	a4,a4,t4
1c00a3f8:	03305c63          	blez	s3,1c00a430 <pos_libc_prf+0x19b6>
1c00a3fc:	4ec1                	li	t4,16
1c00a3fe:	03d51963          	bne	a0,t4,1c00a430 <pos_libc_prf+0x19b6>
1c00a402:	00269e13          	slli	t3,a3,0x2
1c00a406:	01e6de93          	srli	t4,a3,0x1e
1c00a40a:	00271513          	slli	a0,a4,0x2
1c00a40e:	96f2                	add	a3,a3,t3
1c00a410:	00aee533          	or	a0,t4,a0
1c00a414:	01c6b6b3          	sltu	a3,a3,t3
1c00a418:	972a                	add	a4,a4,a0
1c00a41a:	9736                	add	a4,a4,a3
1c00a41c:	0706                	slli	a4,a4,0x1
1c00a41e:	8371                	srli	a4,a4,0x1c
1c00a420:	03070713          	addi	a4,a4,48
1c00a424:	01060e13          	addi	t3,a2,16
1c00a428:	00e607a3          	sb	a4,15(a2)
1c00a42c:	ff030993          	addi	s3,t1,-16
1c00a430:	47e2                	lw	a5,24(sp)
1c00a432:	01578bb3          	add	s7,a5,s5
1c00a436:	96059a63          	bnez	a1,1c0095aa <pos_libc_prf+0xb30>
1c00a43a:	9bce                	add	s7,s7,s3
1c00a43c:	8dce                	mv	s11,s3
1c00a43e:	998ff06f          	j	1c0095d6 <pos_libc_prf+0xb5c>
1c00a442:	02410813          	addi	a6,sp,36
1c00a446:	4581                	li	a1,0
1c00a448:	86c2                	mv	a3,a6
1c00a44a:	e64ff06f          	j	1c009aae <pos_libc_prf+0x1034>
1c00a44e:	02d00713          	li	a4,45
1c00a452:	411008b3          	neg	a7,a7
1c00a456:	00ee00a3          	sb	a4,1(t3)
1c00a45a:	d56ff06f          	j	1c0099b0 <pos_libc_prf+0xf36>
1c00a45e:	4ae2                	lw	s5,24(sp)
1c00a460:	a039                	j	1c00a46e <pos_libc_prf+0x19f4>
1c00a462:	9902                	jalr	s2
1c00a464:	1afd                	addi	s5,s5,-1
1c00a466:	01f53463          	p.bneimm	a0,-1,1c00a46e <pos_libc_prf+0x19f4>
1c00a46a:	edcfe06f          	j	1c008b46 <pos_libc_prf+0xcc>
1c00a46e:	85a6                	mv	a1,s1
1c00a470:	03000513          	li	a0,48
1c00a474:	ff5047e3          	bgtz	s5,1c00a462 <pos_libc_prf+0x19e8>
1c00a478:	47e2                	lw	a5,24(sp)
1c00a47a:	40fd0d33          	sub	s10,s10,a5
1c00a47e:	905fe06f          	j	1c008d82 <pos_libc_prf+0x308>
1c00a482:	41f45713          	srai	a4,s0,0x1f
1c00a486:	c280                	sw	s0,0(a3)
1c00a488:	c2d8                	sw	a4,4(a3)
1c00a48a:	e2cfe06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c00a48e:	47c1                	li	a5,16
1c00a490:	04f9ce33          	p.min	t3,s3,a5
1c00a494:	1e7d                	addi	t3,t3,-1
1c00a496:	89ba                	mv	s3,a4
1c00a498:	4581                	li	a1,0
1c00a49a:	bf9fe06f          	j	1c009092 <pos_libc_prf+0x618>
1c00a49e:	06600c13          	li	s8,102
1c00a4a2:	4701                	li	a4,0
1c00a4a4:	08000fb7          	lui	t6,0x8000
1c00a4a8:	c5bfe06f          	j	1c009102 <pos_libc_prf+0x688>
1c00a4ac:	6719                	lui	a4,0x6
1c00a4ae:	16e70713          	addi	a4,a4,366 # 616e <pos_soc_event_callback+0x5d2e>
1c00a4b2:	00e31023          	sh	a4,0(t1)
1c00a4b6:	06e00713          	li	a4,110
1c00a4ba:	00e30123          	sb	a4,2(t1)
1c00a4be:	cc0ff06f          	j	1c00997e <pos_libc_prf+0xf04>
1c00a4c2:	4af2                	lw	s5,28(sp)
1c00a4c4:	df2fe06f          	j	1c008ab6 <pos_libc_prf+0x3c>
1c00a4c8:	671d                	lui	a4,0x7
1c00a4ca:	e6970713          	addi	a4,a4,-407 # 6e69 <pos_soc_event_callback+0x6a29>
1c00a4ce:	00e31023          	sh	a4,0(t1)
1c00a4d2:	06600713          	li	a4,102
1c00a4d6:	00e30123          	sb	a4,2(t1)
1c00a4da:	ca4ff06f          	j	1c00997e <pos_libc_prf+0xf04>
1c00a4de:	4792                	lw	a5,4(sp)
1c00a4e0:	4581                	li	a1,0
1c00a4e2:	e399                	bnez	a5,1c00a4e8 <pos_libc_prf+0x1a6e>
1c00a4e4:	ba3fe06f          	j	1c009086 <pos_libc_prf+0x60c>
1c00a4e8:	4e01                	li	t3,0
1c00a4ea:	4581                	li	a1,0
1c00a4ec:	ba7fe06f          	j	1c009092 <pos_libc_prf+0x618>
1c00a4f0:	020102a3          	sb	zero,37(sp)
1c00a4f4:	c202                	sw	zero,4(sp)
1c00a4f6:	4b85                	li	s7,1
1c00a4f8:	805fe06f          	j	1c008cfc <pos_libc_prf+0x282>
1c00a4fc:	07304863          	bgtz	s3,1c00a56c <pos_libc_prf+0x1af2>
1c00a500:	4881                	li	a7,0
1c00a502:	cc02                	sw	zero,24(sp)
1c00a504:	b735                	j	1c00a430 <pos_libc_prf+0x19b6>
1c00a506:	4792                	lw	a5,4(sp)
1c00a508:	03000613          	li	a2,48
1c00a50c:	00c30023          	sb	a2,0(t1)
1c00a510:	eb8d                	bnez	a5,1c00a542 <pos_libc_prf+0x1ac8>
1c00a512:	05305263          	blez	s3,1c00a556 <pos_libc_prf+0x1adc>
1c00a516:	02e00613          	li	a2,46
1c00a51a:	00c300a3          	sb	a2,1(t1)
1c00a51e:	00230e13          	addi	t3,t1,2
1c00a522:	4541                	li	a0,16
1c00a524:	04088a63          	beqz	a7,1c00a578 <pos_libc_prf+0x1afe>
1c00a528:	41100633          	neg	a2,a7
1c00a52c:	05364633          	p.min	a2,a2,s3
1c00a530:	cc32                	sw	a2,24(sp)
1c00a532:	40c98333          	sub	t1,s3,a2
1c00a536:	4a81                	li	s5,0
1c00a538:	8672                	mv	a2,t3
1c00a53a:	4541                	li	a0,16
1c00a53c:	a86040e3          	bgtz	t1,1c009fbc <pos_libc_prf+0x1542>
1c00a540:	a8b1                	j	1c00a59c <pos_libc_prf+0x1b22>
1c00a542:	02e00613          	li	a2,46
1c00a546:	00c300a3          	sb	a2,1(t1)
1c00a54a:	00230e13          	addi	t3,t1,2
1c00a54e:	00088763          	beqz	a7,1c00a55c <pos_libc_prf+0x1ae2>
1c00a552:	fd304be3          	bgtz	s3,1c00a528 <pos_libc_prf+0x1aae>
1c00a556:	4a81                	li	s5,0
1c00a558:	cc02                	sw	zero,24(sp)
1c00a55a:	bdd9                	j	1c00a430 <pos_libc_prf+0x19b6>
1c00a55c:	ff305de3          	blez	s3,1c00a556 <pos_libc_prf+0x1adc>
1c00a560:	8672                	mv	a2,t3
1c00a562:	4a81                	li	s5,0
1c00a564:	cc02                	sw	zero,24(sp)
1c00a566:	834e                	mv	t1,s3
1c00a568:	4541                	li	a0,16
1c00a56a:	bc99                	j	1c009fc0 <pos_libc_prf+0x1546>
1c00a56c:	02e00613          	li	a2,46
1c00a570:	00ce0023          	sb	a2,0(t3)
1c00a574:	88d6                	mv	a7,s5
1c00a576:	0e05                	addi	t3,t3,1
1c00a578:	8ac6                	mv	s5,a7
1c00a57a:	8672                	mv	a2,t3
1c00a57c:	834e                	mv	t1,s3
1c00a57e:	cc02                	sw	zero,24(sp)
1c00a580:	4881                	li	a7,0
1c00a582:	bc2d                	j	1c009fbc <pos_libc_prf+0x1542>
1c00a584:	00d70e13          	addi	t3,a4,13
1c00a588:	818ff06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a58c:	00c70e13          	addi	t3,a4,12
1c00a590:	810ff06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a594:	c0200513          	li	a0,-1022
1c00a598:	934ff06f          	j	1c0096cc <pos_libc_prf+0xc52>
1c00a59c:	899a                	mv	s3,t1
1c00a59e:	8e32                	mv	t3,a2
1c00a5a0:	bd41                	j	1c00a430 <pos_libc_prf+0x19b6>
1c00a5a2:	00970e13          	addi	t3,a4,9
1c00a5a6:	ffbfe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5aa:	00870e13          	addi	t3,a4,8
1c00a5ae:	ff3fe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5b2:	00770e13          	addi	t3,a4,7
1c00a5b6:	febfe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5ba:	00670e13          	addi	t3,a4,6
1c00a5be:	fe3fe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5c2:	00570e13          	addi	t3,a4,5
1c00a5c6:	fdbfe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5ca:	00470e13          	addi	t3,a4,4
1c00a5ce:	fd3fe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5d2:	00370e13          	addi	t3,a4,3
1c00a5d6:	fcbfe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5da:	00270e13          	addi	t3,a4,2
1c00a5de:	fc3fe06f          	j	1c0095a0 <pos_libc_prf+0xb26>
1c00a5e2:	00c30e13          	addi	t3,t1,12
1c00a5e6:	ba6d                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a5e8:	00b30e13          	addi	t3,t1,11
1c00a5ec:	4515                	li	a0,5
1c00a5ee:	ba4d                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a5f0:	00a30e13          	addi	t3,t1,10
1c00a5f4:	4519                	li	a0,6
1c00a5f6:	b26d                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a5f8:	00930e13          	addi	t3,t1,9
1c00a5fc:	451d                	li	a0,7
1c00a5fe:	b24d                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a600:	00830e13          	addi	t3,t1,8
1c00a604:	4521                	li	a0,8
1c00a606:	ba69                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a608:	00730e13          	addi	t3,t1,7
1c00a60c:	4525                	li	a0,9
1c00a60e:	ba49                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a610:	00630e13          	addi	t3,t1,6
1c00a614:	4529                	li	a0,10
1c00a616:	b269                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a618:	00530e13          	addi	t3,t1,5
1c00a61c:	452d                	li	a0,11
1c00a61e:	b249                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a620:	00430e13          	addi	t3,t1,4
1c00a624:	4531                	li	a0,12
1c00a626:	baad                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a628:	00330e13          	addi	t3,t1,3
1c00a62c:	4535                	li	a0,13
1c00a62e:	ba8d                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a630:	00230e13          	addi	t3,t1,2
1c00a634:	4a81                	li	s5,0
1c00a636:	4539                	li	a0,14
1c00a638:	b2a5                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a63a:	00e30e13          	addi	t3,t1,14
1c00a63e:	4509                	li	a0,2
1c00a640:	b285                	j	1c009fa0 <pos_libc_prf+0x1526>
1c00a642:	4a81                	li	s5,0
1c00a644:	453d                	li	a0,15
1c00a646:	baa9                	j	1c009fa0 <pos_libc_prf+0x1526>

1c00a648 <pos_init_start>:
1c00a648:	1141                	addi	sp,sp,-16
1c00a64a:	c422                	sw	s0,8(sp)
1c00a64c:	1c001437          	lui	s0,0x1c001
1c00a650:	c606                	sw	ra,12(sp)
1c00a652:	84440413          	addi	s0,s0,-1980 # 1c000844 <ctor_list>
1c00a656:	2215                	jal	1c00a77a <pos_irq_init>
1c00a658:	b4afe0ef          	jal	ra,1c0089a2 <pos_soc_init>
1c00a65c:	22a9                	jal	1c00a7a6 <pos_soc_event_init>
1c00a65e:	2069                	jal	1c00a6e8 <pos_allocs_init>
1c00a660:	208d                	jal	1c00a6c2 <pos_sched_init>
1c00a662:	405c                	lw	a5,4(s0)
1c00a664:	c791                	beqz	a5,1c00a670 <pos_init_start+0x28>
1c00a666:	0421                	addi	s0,s0,8
1c00a668:	9782                	jalr	a5
1c00a66a:	0044278b          	p.lw	a5,4(s0!)
1c00a66e:	ffed                	bnez	a5,1c00a668 <pos_init_start+0x20>
1c00a670:	bd4fe0ef          	jal	ra,1c008a44 <pos_io_start>
1c00a674:	300467f3          	csrrsi	a5,mstatus,8
1c00a678:	40b2                	lw	ra,12(sp)
1c00a67a:	4422                	lw	s0,8(sp)
1c00a67c:	0141                	addi	sp,sp,16
1c00a67e:	8082                	ret

1c00a680 <pos_init_stop>:
1c00a680:	1141                	addi	sp,sp,-16
1c00a682:	c422                	sw	s0,8(sp)
1c00a684:	1c001437          	lui	s0,0x1c001
1c00a688:	c606                	sw	ra,12(sp)
1c00a68a:	85840413          	addi	s0,s0,-1960 # 1c000858 <dtor_list>
1c00a68e:	bbafe0ef          	jal	ra,1c008a48 <pos_io_stop>
1c00a692:	405c                	lw	a5,4(s0)
1c00a694:	c791                	beqz	a5,1c00a6a0 <pos_init_stop+0x20>
1c00a696:	0421                	addi	s0,s0,8
1c00a698:	9782                	jalr	a5
1c00a69a:	0044278b          	p.lw	a5,4(s0!)
1c00a69e:	ffed                	bnez	a5,1c00a698 <pos_init_stop+0x18>
1c00a6a0:	40b2                	lw	ra,12(sp)
1c00a6a2:	4422                	lw	s0,8(sp)
1c00a6a4:	0141                	addi	sp,sp,16
1c00a6a6:	8082                	ret

1c00a6a8 <pos_cbsys_add>:
1c00a6a8:	00259793          	slli	a5,a1,0x2
1c00a6ac:	1c0015b7          	lui	a1,0x1c001
1c00a6b0:	20058593          	addi	a1,a1,512 # 1c001200 <stack>
1c00a6b4:	95be                	add	a1,a1,a5
1c00a6b6:	419c                	lw	a5,0(a1)
1c00a6b8:	c110                	sw	a2,0(a0)
1c00a6ba:	c154                	sw	a3,4(a0)
1c00a6bc:	c51c                	sw	a5,8(a0)
1c00a6be:	c188                	sw	a0,0(a1)
1c00a6c0:	8082                	ret

1c00a6c2 <pos_sched_init>:
1c00a6c2:	00002c23          	sw	zero,24(zero) # 18 <pos_sched_first>
1c00a6c6:	8082                	ret

1c00a6c8 <pos_alloc_init>:
1c00a6c8:	00758793          	addi	a5,a1,7
1c00a6cc:	c407b7b3          	p.bclr	a5,a5,2,0
1c00a6d0:	40b785b3          	sub	a1,a5,a1
1c00a6d4:	c11c                	sw	a5,0(a0)
1c00a6d6:	8e0d                	sub	a2,a2,a1
1c00a6d8:	00c05763          	blez	a2,1c00a6e6 <pos_alloc_init+0x1e>
1c00a6dc:	c4063633          	p.bclr	a2,a2,2,0
1c00a6e0:	c390                	sw	a2,0(a5)
1c00a6e2:	0007a223          	sw	zero,4(a5) # ffff8004 <pulp__FC+0xffff8005>
1c00a6e6:	8082                	ret

1c00a6e8 <pos_allocs_init>:
1c00a6e8:	1141                	addi	sp,sp,-16
1c00a6ea:	1c0025b7          	lui	a1,0x1c002
1c00a6ee:	c606                	sw	ra,12(sp)
1c00a6f0:	84858793          	addi	a5,a1,-1976 # 1c001848 <__l2_priv0_end>
1c00a6f4:	1c008637          	lui	a2,0x1c008
1c00a6f8:	02c7c963          	blt	a5,a2,1c00a72a <pos_allocs_init+0x42>
1c00a6fc:	4581                	li	a1,0
1c00a6fe:	4601                	li	a2,0
1c00a700:	1c002537          	lui	a0,0x1c002
1c00a704:	84050513          	addi	a0,a0,-1984 # 1c001840 <pos_alloc_l2>
1c00a708:	37c1                	jal	1c00a6c8 <pos_alloc_init>
1c00a70a:	1c0105b7          	lui	a1,0x1c010
1c00a70e:	40b2                	lw	ra,12(sp)
1c00a710:	03858793          	addi	a5,a1,56 # 1c010038 <__l2_shared_end>
1c00a714:	1c080637          	lui	a2,0x1c080
1c00a718:	1c002537          	lui	a0,0x1c002
1c00a71c:	8e1d                	sub	a2,a2,a5
1c00a71e:	03858593          	addi	a1,a1,56
1c00a722:	84450513          	addi	a0,a0,-1980 # 1c001844 <pos_alloc_l2+0x4>
1c00a726:	0141                	addi	sp,sp,16
1c00a728:	b745                	j	1c00a6c8 <pos_alloc_init>
1c00a72a:	8e1d                	sub	a2,a2,a5
1c00a72c:	84858593          	addi	a1,a1,-1976
1c00a730:	bfc1                	j	1c00a700 <pos_allocs_init+0x18>

1c00a732 <pos_irq_set_handler>:
1c00a732:	f14027f3          	csrr	a5,mhartid
1c00a736:	46fd                	li	a3,31
1c00a738:	4701                	li	a4,0
1c00a73a:	ca5797b3          	p.extractu	a5,a5,5,5
1c00a73e:	00d79663          	bne	a5,a3,1c00a74a <pos_irq_set_handler+0x18>
1c00a742:	30502773          	csrr	a4,mtvec
1c00a746:	c0073733          	p.bclr	a4,a4,0,0
1c00a74a:	050a                	slli	a0,a0,0x2
1c00a74c:	8d89                	sub	a1,a1,a0
1c00a74e:	8d99                	sub	a1,a1,a4
1c00a750:	c14586b3          	p.extract	a3,a1,0,20
1c00a754:	06f00793          	li	a5,111
1c00a758:	c1f6a7b3          	p.insert	a5,a3,0,31
1c00a75c:	d21586b3          	p.extract	a3,a1,9,1
1c00a760:	d356a7b3          	p.insert	a5,a3,9,21
1c00a764:	c0b586b3          	p.extract	a3,a1,0,11
1c00a768:	c146a7b3          	p.insert	a5,a3,0,20
1c00a76c:	cec585b3          	p.extract	a1,a1,7,12
1c00a770:	cec5a7b3          	p.insert	a5,a1,7,12
1c00a774:	00f56723          	p.sw	a5,a4(a0)
1c00a778:	8082                	ret

1c00a77a <pos_irq_init>:
1c00a77a:	f14027f3          	csrr	a5,mhartid
1c00a77e:	1a10a737          	lui	a4,0x1a10a
1c00a782:	56fd                	li	a3,-1
1c00a784:	80d72423          	sw	a3,-2040(a4) # 1a109808 <__l1_end+0xa109804>
1c00a788:	ca5797b3          	p.extractu	a5,a5,5,5
1c00a78c:	477d                	li	a4,31
1c00a78e:	00e78363          	beq	a5,a4,1c00a794 <pos_irq_init+0x1a>
1c00a792:	8082                	ret
1c00a794:	1c0087b7          	lui	a5,0x1c008
1c00a798:	00078793          	mv	a5,a5
1c00a79c:	c007c7b3          	p.bset	a5,a5,0,0
1c00a7a0:	30579073          	csrw	mtvec,a5
1c00a7a4:	8082                	ret

1c00a7a6 <pos_soc_event_init>:
1c00a7a6:	1a1067b7          	lui	a5,0x1a106
1c00a7aa:	1141                	addi	sp,sp,-16
1c00a7ac:	c606                	sw	ra,12(sp)
1c00a7ae:	577d                	li	a4,-1
1c00a7b0:	00478693          	addi	a3,a5,4 # 1a106004 <__l1_end+0xa106000>
1c00a7b4:	c298                	sw	a4,0(a3)
1c00a7b6:	00878693          	addi	a3,a5,8
1c00a7ba:	c298                	sw	a4,0(a3)
1c00a7bc:	00c78693          	addi	a3,a5,12
1c00a7c0:	c298                	sw	a4,0(a3)
1c00a7c2:	01078693          	addi	a3,a5,16
1c00a7c6:	c298                	sw	a4,0(a3)
1c00a7c8:	01478693          	addi	a3,a5,20
1c00a7cc:	c298                	sw	a4,0(a3)
1c00a7ce:	01878693          	addi	a3,a5,24
1c00a7d2:	c298                	sw	a4,0(a3)
1c00a7d4:	01c78693          	addi	a3,a5,28
1c00a7d8:	c298                	sw	a4,0(a3)
1c00a7da:	02078793          	addi	a5,a5,32
1c00a7de:	1c00b5b7          	lui	a1,0x1c00b
1c00a7e2:	c398                	sw	a4,0(a5)
1c00a7e4:	a6c58593          	addi	a1,a1,-1428 # 1c00aa6c <pos_soc_event_handler_asm>
1c00a7e8:	4569                	li	a0,26
1c00a7ea:	37a1                	jal	1c00a732 <pos_irq_set_handler>
1c00a7ec:	40b2                	lw	ra,12(sp)
1c00a7ee:	1a10a7b7          	lui	a5,0x1a10a
1c00a7f2:	04000737          	lui	a4,0x4000
1c00a7f6:	80e7a223          	sw	a4,-2044(a5) # 1a109804 <__l1_end+0xa109800>
1c00a7fa:	0141                	addi	sp,sp,16
1c00a7fc:	8082                	ret

1c00a7fe <pos_time_poweroff>:
1c00a7fe:	1a10b7b7          	lui	a5,0x1a10b
1c00a802:	0791                	addi	a5,a5,4
1c00a804:	0087a783          	lw	a5,8(a5) # 1a10b008 <__l1_end+0xa10b004>
1c00a808:	1c001737          	lui	a4,0x1c001
1c00a80c:	20f72c23          	sw	a5,536(a4) # 1c001218 <pos_time_timer_count>
1c00a810:	4501                	li	a0,0
1c00a812:	8082                	ret

1c00a814 <pos_time_poweron>:
1c00a814:	1c0017b7          	lui	a5,0x1c001
1c00a818:	2187a703          	lw	a4,536(a5) # 1c001218 <pos_time_timer_count>
1c00a81c:	1a10b7b7          	lui	a5,0x1a10b
1c00a820:	0791                	addi	a5,a5,4
1c00a822:	00e7a423          	sw	a4,8(a5) # 1a10b008 <__l1_end+0xa10b004>
1c00a826:	4501                	li	a0,0
1c00a828:	8082                	ret

1c00a82a <pos_time_timer_handler>:
1c00a82a:	1c0018b7          	lui	a7,0x1c001
1c00a82e:	21888893          	addi	a7,a7,536 # 1c001218 <pos_time_timer_count>
1c00a832:	0048a783          	lw	a5,4(a7)
1c00a836:	1a10b637          	lui	a2,0x1a10b
1c00a83a:	0611                	addi	a2,a2,4
1c00a83c:	00862603          	lw	a2,8(a2) # 1a10b008 <__l1_end+0xa10b004>
1c00a840:	cba9                	beqz	a5,1c00a892 <pos_time_timer_handler+0x68>
1c00a842:	5fd8                	lw	a4,60(a5)
1c00a844:	800005b7          	lui	a1,0x80000
1c00a848:	ffe5c593          	xori	a1,a1,-2
1c00a84c:	40e60733          	sub	a4,a2,a4
1c00a850:	08e5e863          	bltu	a1,a4,1c00a8e0 <pos_time_timer_handler+0xb6>
1c00a854:	01802803          	lw	a6,24(zero) # 18 <pos_sched_first>
1c00a858:	01c02503          	lw	a0,28(zero) # 1c <pos_sched_last>
1c00a85c:	4398                	lw	a4,0(a5)
1c00a85e:	0007a023          	sw	zero,0(a5)
1c00a862:	4301                	li	t1,0
1c00a864:	02080063          	beqz	a6,1c00a884 <pos_time_timer_handler+0x5a>
1c00a868:	c11c                	sw	a5,0(a0)
1c00a86a:	c305                	beqz	a4,1c00a88a <pos_time_timer_handler+0x60>
1c00a86c:	5f54                	lw	a3,60(a4)
1c00a86e:	853e                	mv	a0,a5
1c00a870:	40d606b3          	sub	a3,a2,a3
1c00a874:	04d5e063          	bltu	a1,a3,1c00a8b4 <pos_time_timer_handler+0x8a>
1c00a878:	87ba                	mv	a5,a4
1c00a87a:	4398                	lw	a4,0(a5)
1c00a87c:	0007a023          	sw	zero,0(a5)
1c00a880:	fe0814e3          	bnez	a6,1c00a868 <pos_time_timer_handler+0x3e>
1c00a884:	883e                	mv	a6,a5
1c00a886:	4305                	li	t1,1
1c00a888:	f375                	bnez	a4,1c00a86c <pos_time_timer_handler+0x42>
1c00a88a:	06031063          	bnez	t1,1c00a8ea <pos_time_timer_handler+0xc0>
1c00a88e:	00f02e23          	sw	a5,28(zero) # 1c <pos_sched_last>
1c00a892:	1a10b7b7          	lui	a5,0x1a10b
1c00a896:	08100713          	li	a4,129
1c00a89a:	0791                	addi	a5,a5,4
1c00a89c:	0008a223          	sw	zero,4(a7)
1c00a8a0:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10affc>
1c00a8a4:	6785                	lui	a5,0x1
1c00a8a6:	1a10a737          	lui	a4,0x1a10a
1c00a8aa:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3c0>
1c00a8ae:	80f72a23          	sw	a5,-2028(a4) # 1a109814 <__l1_end+0xa109810>
1c00a8b2:	8082                	ret
1c00a8b4:	02031863          	bnez	t1,1c00a8e4 <pos_time_timer_handler+0xba>
1c00a8b8:	00f02e23          	sw	a5,28(zero) # 1c <pos_sched_last>
1c00a8bc:	1a10b7b7          	lui	a5,0x1a10b
1c00a8c0:	0791                	addi	a5,a5,4
1c00a8c2:	00e8a223          	sw	a4,4(a7)
1c00a8c6:	0087a683          	lw	a3,8(a5) # 1a10b008 <__l1_end+0xa10b004>
1c00a8ca:	5f58                	lw	a4,60(a4)
1c00a8cc:	40c70633          	sub	a2,a4,a2
1c00a8d0:	9636                	add	a2,a2,a3
1c00a8d2:	00c7a823          	sw	a2,16(a5)
1c00a8d6:	08500713          	li	a4,133
1c00a8da:	00e7a023          	sw	a4,0(a5)
1c00a8de:	8082                	ret
1c00a8e0:	873e                	mv	a4,a5
1c00a8e2:	bfe9                	j	1c00a8bc <pos_time_timer_handler+0x92>
1c00a8e4:	01002c23          	sw	a6,24(zero) # 18 <pos_sched_first>
1c00a8e8:	bfc1                	j	1c00a8b8 <pos_time_timer_handler+0x8e>
1c00a8ea:	01002c23          	sw	a6,24(zero) # 18 <pos_sched_first>
1c00a8ee:	b745                	j	1c00a88e <pos_time_timer_handler+0x64>

1c00a8f0 <pos_time_init>:
1c00a8f0:	1141                	addi	sp,sp,-16
1c00a8f2:	c422                	sw	s0,8(sp)
1c00a8f4:	1a10b7b7          	lui	a5,0x1a10b
1c00a8f8:	1c001437          	lui	s0,0x1c001
1c00a8fc:	c606                	sw	ra,12(sp)
1c00a8fe:	21840413          	addi	s0,s0,536 # 1c001218 <pos_time_timer_count>
1c00a902:	08300713          	li	a4,131
1c00a906:	0791                	addi	a5,a5,4
1c00a908:	00042223          	sw	zero,4(s0)
1c00a90c:	00e7a023          	sw	a4,0(a5) # 1a10b000 <__l1_end+0xa10affc>
1c00a910:	1c00b5b7          	lui	a1,0x1c00b
1c00a914:	a4858593          	addi	a1,a1,-1464 # 1c00aa48 <pos_time_timer_handler_asm>
1c00a918:	452d                	li	a0,11
1c00a91a:	3d21                	jal	1c00a732 <pos_irq_set_handler>
1c00a91c:	6785                	lui	a5,0x1
1c00a91e:	1a10a737          	lui	a4,0x1a10a
1c00a922:	80078793          	addi	a5,a5,-2048 # 800 <pos_soc_event_callback+0x3c0>
1c00a926:	1c00a637          	lui	a2,0x1c00a
1c00a92a:	00840513          	addi	a0,s0,8
1c00a92e:	4681                	li	a3,0
1c00a930:	7fe60613          	addi	a2,a2,2046 # 1c00a7fe <pos_time_poweroff>
1c00a934:	4589                	li	a1,2
1c00a936:	80f72223          	sw	a5,-2044(a4) # 1a109804 <__l1_end+0xa109800>
1c00a93a:	33bd                	jal	1c00a6a8 <pos_cbsys_add>
1c00a93c:	01840513          	addi	a0,s0,24
1c00a940:	4422                	lw	s0,8(sp)
1c00a942:	40b2                	lw	ra,12(sp)
1c00a944:	1c00b637          	lui	a2,0x1c00b
1c00a948:	4681                	li	a3,0
1c00a94a:	81460613          	addi	a2,a2,-2028 # 1c00a814 <pos_time_poweron>
1c00a94e:	458d                	li	a1,3
1c00a950:	0141                	addi	sp,sp,16
1c00a952:	bb99                	j	1c00a6a8 <pos_cbsys_add>

1c00a954 <pos_uart_init>:
1c00a954:	1c0107b7          	lui	a5,0x1c010
1c00a958:	0007a023          	sw	zero,0(a5) # 1c010000 <__cluster_text_end>
1c00a95c:	8082                	ret

1c00a95e <pos_init_entry>:
1c00a95e:	7a101073          	csrw	pcmr,zero
1c00a962:	ffff7297          	auipc	t0,0xffff7
1c00a966:	8de28293          	addi	t0,t0,-1826 # 1c001240 <_edata>
1c00a96a:	ffff7317          	auipc	t1,0xffff7
1c00a96e:	ede30313          	addi	t1,t1,-290 # 1c001848 <__l2_priv0_end>
1c00a972:	0002a023          	sw	zero,0(t0)
1c00a976:	0291                	addi	t0,t0,4
1c00a978:	fe62ede3          	bltu	t0,t1,1c00a972 <pos_init_entry+0x14>
1c00a97c:	ffff7117          	auipc	sp,0xffff7
1c00a980:	88410113          	addi	sp,sp,-1916 # 1c001200 <stack>
1c00a984:	00000297          	auipc	t0,0x0
1c00a988:	cc428293          	addi	t0,t0,-828 # 1c00a648 <pos_init_start>
1c00a98c:	000280e7          	jalr	t0
1c00a990:	00000513          	li	a0,0
1c00a994:	00000593          	li	a1,0
1c00a998:	ffffe397          	auipc	t2,0xffffe
1c00a99c:	e1238393          	addi	t2,t2,-494 # 1c0087aa <main>
1c00a9a0:	000380e7          	jalr	t2
1c00a9a4:	842a                	mv	s0,a0
1c00a9a6:	8522                	mv	a0,s0
1c00a9a8:	ffffe297          	auipc	t0,0xffffe
1c00a9ac:	07028293          	addi	t0,t0,112 # 1c008a18 <exit>
1c00a9b0:	000280e7          	jalr	t0

1c00a9b4 <pos_irq_call_external_c_function>:
1c00a9b4:	7119                	addi	sp,sp,-128
1c00a9b6:	c006                	sw	ra,0(sp)
1c00a9b8:	c20e                	sw	gp,4(sp)
1c00a9ba:	c412                	sw	tp,8(sp)
1c00a9bc:	c616                	sw	t0,12(sp)
1c00a9be:	c81a                	sw	t1,16(sp)
1c00a9c0:	ca1e                	sw	t2,20(sp)
1c00a9c2:	d236                	sw	a3,36(sp)
1c00a9c4:	d43a                	sw	a4,40(sp)
1c00a9c6:	d63e                	sw	a5,44(sp)
1c00a9c8:	d842                	sw	a6,48(sp)
1c00a9ca:	da46                	sw	a7,52(sp)
1c00a9cc:	dc72                	sw	t3,56(sp)
1c00a9ce:	de76                	sw	t4,60(sp)
1c00a9d0:	c0fa                	sw	t5,64(sp)
1c00a9d2:	c6fe                	sw	t6,76(sp)
1c00a9d4:	000600e7          	jalr	a2
1c00a9d8:	4082                	lw	ra,0(sp)
1c00a9da:	4192                	lw	gp,4(sp)
1c00a9dc:	4222                	lw	tp,8(sp)
1c00a9de:	42b2                	lw	t0,12(sp)
1c00a9e0:	4342                	lw	t1,16(sp)
1c00a9e2:	43d2                	lw	t2,20(sp)
1c00a9e4:	5692                	lw	a3,36(sp)
1c00a9e6:	5722                	lw	a4,40(sp)
1c00a9e8:	57b2                	lw	a5,44(sp)
1c00a9ea:	5842                	lw	a6,48(sp)
1c00a9ec:	58d2                	lw	a7,52(sp)
1c00a9ee:	5e62                	lw	t3,56(sp)
1c00a9f0:	5ef2                	lw	t4,60(sp)
1c00a9f2:	4f06                	lw	t5,64(sp)
1c00a9f4:	4fb6                	lw	t6,76(sp)
1c00a9f6:	6109                	addi	sp,sp,128
1c00a9f8:	8482                	jr	s1

1c00a9fa <pos_irq_call_external_c_function_full>:
1c00a9fa:	7119                	addi	sp,sp,-128
1c00a9fc:	c006                	sw	ra,0(sp)
1c00a9fe:	c20e                	sw	gp,4(sp)
1c00aa00:	c412                	sw	tp,8(sp)
1c00aa02:	c616                	sw	t0,12(sp)
1c00aa04:	c81a                	sw	t1,16(sp)
1c00aa06:	ca1e                	sw	t2,20(sp)
1c00aa08:	cc2a                	sw	a0,24(sp)
1c00aa0a:	ce2e                	sw	a1,28(sp)
1c00aa0c:	d236                	sw	a3,36(sp)
1c00aa0e:	d43a                	sw	a4,40(sp)
1c00aa10:	d63e                	sw	a5,44(sp)
1c00aa12:	d842                	sw	a6,48(sp)
1c00aa14:	da46                	sw	a7,52(sp)
1c00aa16:	dc72                	sw	t3,56(sp)
1c00aa18:	de76                	sw	t4,60(sp)
1c00aa1a:	c0fa                	sw	t5,64(sp)
1c00aa1c:	c6fe                	sw	t6,76(sp)
1c00aa1e:	000600e7          	jalr	a2
1c00aa22:	4082                	lw	ra,0(sp)
1c00aa24:	4192                	lw	gp,4(sp)
1c00aa26:	4222                	lw	tp,8(sp)
1c00aa28:	42b2                	lw	t0,12(sp)
1c00aa2a:	4342                	lw	t1,16(sp)
1c00aa2c:	43d2                	lw	t2,20(sp)
1c00aa2e:	4562                	lw	a0,24(sp)
1c00aa30:	45f2                	lw	a1,28(sp)
1c00aa32:	5692                	lw	a3,36(sp)
1c00aa34:	5722                	lw	a4,40(sp)
1c00aa36:	57b2                	lw	a5,44(sp)
1c00aa38:	5842                	lw	a6,48(sp)
1c00aa3a:	58d2                	lw	a7,52(sp)
1c00aa3c:	5e62                	lw	t3,56(sp)
1c00aa3e:	5ef2                	lw	t4,60(sp)
1c00aa40:	4f06                	lw	t5,64(sp)
1c00aa42:	4fb6                	lw	t6,76(sp)
1c00aa44:	6109                	addi	sp,sp,128
1c00aa46:	8482                	jr	s1

1c00aa48 <pos_time_timer_handler_asm>:
1c00aa48:	1161                	addi	sp,sp,-8
1c00aa4a:	c032                	sw	a2,0(sp)
1c00aa4c:	c226                	sw	s1,4(sp)
1c00aa4e:	00000617          	auipc	a2,0x0
1c00aa52:	ddc60613          	addi	a2,a2,-548 # 1c00a82a <pos_time_timer_handler>
1c00aa56:	00000497          	auipc	s1,0x0
1c00aa5a:	00c48493          	addi	s1,s1,12 # 1c00aa62 <pos_time_timer_handler_asm_ret>
1c00aa5e:	f9dff06f          	j	1c00a9fa <pos_irq_call_external_c_function_full>

1c00aa62 <pos_time_timer_handler_asm_ret>:
1c00aa62:	4492                	lw	s1,4(sp)
1c00aa64:	4602                	lw	a2,0(sp)
1c00aa66:	0121                	addi	sp,sp,8
1c00aa68:	30200073          	mret

1c00aa6c <pos_soc_event_handler_asm>:
1c00aa6c:	7119                	addi	sp,sp,-128
1c00aa6e:	c022                	sw	s0,0(sp)
1c00aa70:	c226                	sw	s1,4(sp)
1c00aa72:	c42a                	sw	a0,8(sp)
1c00aa74:	c62e                	sw	a1,12(sp)
1c00aa76:	c832                	sw	a2,16(sp)
1c00aa78:	1a10a437          	lui	s0,0x1a10a
1c00aa7c:	80040413          	addi	s0,s0,-2048 # 1a109800 <__l1_end+0xa1097fc>
1c00aa80:	5048                	lw	a0,36(s0)
1c00aa82:	00251593          	slli	a1,a0,0x2
1c00aa86:	e3ff6297          	auipc	t0,0xe3ff6
1c00aa8a:	9ba28293          	addi	t0,t0,-1606 # 440 <pos_soc_event_callback>
1c00aa8e:	2055f603          	p.lw	a2,t0(a1)
1c00aa92:	e3ff5297          	auipc	t0,0xe3ff5
1c00aa96:	5ae28293          	addi	t0,t0,1454 # 40 <pos_soc_event_callback_arg>
1c00aa9a:	2055f583          	p.lw	a1,t0(a1)
1c00aa9e:	00000497          	auipc	s1,0x0
1c00aaa2:	03048493          	addi	s1,s1,48 # 1c00aace <pos_soc_event_handler_end_asm>
1c00aaa6:	f0fff06f          	j	1c00a9b4 <pos_irq_call_external_c_function>

1c00aaaa <pos_soc_event_no_udma_asm>:
1c00aaaa:	00555593          	srli	a1,a0,0x5
1c00aaae:	058a                	slli	a1,a1,0x2
1c00aab0:	e3ff5297          	auipc	t0,0xe3ff5
1c00aab4:	57028293          	addi	t0,t0,1392 # 20 <pos_soc_event_status>
1c00aab8:	2055f603          	p.lw	a2,t0(a1)
1c00aabc:	897d                	andi	a0,a0,31
1c00aabe:	80a64633          	p.bsetr	a2,a2,a0
1c00aac2:	e3ff5297          	auipc	t0,0xe3ff5
1c00aac6:	55e28293          	addi	t0,t0,1374 # 20 <pos_soc_event_status>
1c00aaca:	00c5e2a3          	p.sw	a2,t0(a1)

1c00aace <pos_soc_event_handler_end_asm>:
1c00aace:	4402                	lw	s0,0(sp)
1c00aad0:	4492                	lw	s1,4(sp)
1c00aad2:	4522                	lw	a0,8(sp)
1c00aad4:	45b2                	lw	a1,12(sp)
1c00aad6:	4642                	lw	a2,16(sp)
1c00aad8:	6109                	addi	sp,sp,128
1c00aada:	30200073          	mret

1c00aade <_endtext>:
	...
