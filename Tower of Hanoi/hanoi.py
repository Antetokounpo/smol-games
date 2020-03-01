
class Hanoi:
    def __init__(self, n: int):
        self.stack_a = list()
        self.stack_b = list()
        self.stack_c = list()

        self.n = n
        
        # Stack les disques sur la première tour
        self.stack_a = [i+1 for i in reversed(range(self.n))]

    @staticmethod
    def legal_move(a: list, b: list):
        # Si les deux piles sont vides
        if not a and not b:
            return False

        # Savoir dans quelle "direction" le disque doit aller quand on fait un move
        if not a:
            moved = b.pop()
            a.append(moved)
            return
        elif not b:
            moved = a.pop()
            b.append(moved)
            return
        elif a[-1] < b[-1]:
            moved = a.pop()
            b.append(moved)
            return
        elif b[-1] < a[-1]:
            moved = b.pop()
            a.append(moved)
            return

    def solve(self):
        if self.n % 2 == 0:
            while True:
                print(self.stack_a)
                print(self.stack_b)
                print(self.stack_c)
                print('\n')
                
                self.legal_move(self.stack_a, self.stack_b)
                if len(self.stack_c) == self.n:
                    break
                self.legal_move(self.stack_a, self.stack_c)
                if len(self.stack_c) == self.n:
                    break
                self.legal_move(self.stack_b, self.stack_c)
                if len(self.stack_c) == self.n:
                    break
        else:
            while True:
                print(self.stack_a)
                print(self.stack_b)
                print(self.stack_c)
                print('\n')
                
                self.legal_move(self.stack_a, self.stack_c)
                if len(self.stack_c) == self.n:
                    break
                self.legal_move(self.stack_a, self.stack_b)
                if len(self.stack_c) == self.n:
                    break
                self.legal_move(self.stack_b, self.stack_c)
                if len(self.stack_c) == self.n:
                    break

def main():
    hanoi = Hanoi(32)

    hanoi.solve()
    print(hanoi.stack_c)

if __name__ == '__main__':
    main()      
                    