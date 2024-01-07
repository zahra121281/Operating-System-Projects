compile:
	gcc -lpthread -lrt producer.c -o producer
	gcc -lpthread -lrt consumer.c -o consumer
	gcc -lpthread -lrt controller.c -o controller
run: run_producer run_controller
run_producer:
	./producer
run_controller:
	sleep 5
	./controller

