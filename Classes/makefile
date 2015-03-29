msgbin:
	./proto_t.awk  proto.txt
	g++ -o msgbin MsgTest.cpp MsgBin.cpp MsgGen.cpp  ; ./msgbin

socknet:
	cp /Users/zhuoyikang/bubble_go/src/bubble/*.txt ./
	./proto_t.awk  proto.txt
	g++ -DSELF_MAIN -o SockNet SockNet.cpp MsgBin.cpp MsgGen.cpp ; ./SockNet

run:
	./SockNet

g:
	./proto_t.awk  proto.txt

.PHONY:msgbin socknet g
