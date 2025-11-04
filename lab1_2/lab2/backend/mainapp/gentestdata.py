import random, datetime
from .models import *
from django.db import transaction
from faker import Faker

fk = Faker()

price_ranges = {
    'BTC': (100000.0, 200000.0),
    'ETH': (3000.0, 4000.0),
    'SOL': (100.0, 200.0),
    'XRP': (2.0, 3.0),
}

def gentestdata(n):
    with transaction.atomic():
        for _ in range(n):
            newOrder = Order()
            newOrder.user = random.randint(1000, 9999)
            newOrder.base = random.choice(list(price_ranges.keys()))
            newOrder.quote = 'USDT'
            newOrder.side = random.choice(['BUY', 'SELL'])
            newOrder.size = round(random.uniform(0.0, 10.0), 2)
            newOrder.price = round(random.uniform(*price_ranges[newOrder.base]), 2)
            newOrder.is_processed = random.choice([True, False])
            newOrder.opened_at = fk.date_time_this_month(tzinfo=datetime.timezone.utc)
            newOrder.save()
