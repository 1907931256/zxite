r'''>>> import SinkWorld
>>> x = SinkWorld.SplitVector_1()
>>> x.Insert(0,2)
>>> x.CharAt(0)
2
>>> x.CharAt(2)
Traceback (most recent call last):
  File "<interactive input>", line 1, in ?
UserWarning: BadPosition
>>> 
'''

def run(args = None):
    if args is not None:
        import sys
        sys.argv = args
    import doctest, test_SinkWorld
    return doctest.testmod(test_SinkWorld)

if __name__ == '__main__':
    import sys
    sys.exit(run()[0])
