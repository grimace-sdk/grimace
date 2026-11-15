char order[16];
volatile int order_length;

void record(char step) {
    if (order_length < 15) {
        order[order_length] = step;
        order_length = order_length + 1;
    }
}

struct recorder {
    recorder(char step) {
        record(step);
    }
};

static recorder other('b');
