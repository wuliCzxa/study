LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY EDA2000 IS
PORT (
  clk       : IN    STD_LOGIC;                     -- 主时钟输入      接IO30
  rst       : IN    STD_LOGIC;                     -- 复位输入        接IO48
  keyin     : IN    STD_LOGIC_VECTOR(7 DOWNTO 0);  -- 键输入          接IO47、IO46、IO45、IO44、IO43、IO42、IO41、IO40
  portout   : OUT   STD_LOGIC_VECTOR(7 DOWNTO 0);  -- 端口输出        接IO39、IO38、IO37、IO36、IO35、IO34、IO33、IO32

  mem_d     : INOUT STD_LOGIC_VECTOR(7  DOWNTO 0); -- 存贮器数据线    接IO15、IO14、IO13、IO12、IO11、IO10、IO09、IO08
  mem_a     : OUT   STD_LOGIC_VECTOR(14 DOWNTO 0); -- 存贮器地址线    接IO06、IO05、IO04、IO03、IO02、IO01、IO00
                                                   --                   IO63、IO62、IO61、IO60、IO59、IO58、IO57、IO56
                                                   --                   只使用低8位，高7位接'0'
  mem_rd    : OUT   STD_LOGIC;                     -- 存贮器读信号    接IO16
  mem_wr    : OUT   STD_LOGIC;                     -- 存贮器写信号    接IO17

  i_req     : IN    STD_LOGIC                      -- 中断请求信号    接IO49
);
END EDA2000;

ARCHITECTURE behv OF EDA2000 IS

-- 寄存器定义
SIGNAL A   : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL W   : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL R0  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL R1  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL R2  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL R3  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL PC  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL MAR : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL ST  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL IA  : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL IR  : STD_LOGIC_VECTOR(7 DOWNTO 0);

-- 标志定义
SIGNAL R_CY: STD_LOGIC; -- 进位标志寄存器
SIGNAL R_Z : STD_LOGIC; -- 零标志寄存器
SIGNAL CY  : STD_LOGIC; -- 本次运算进位标志
SIGNAL Z   : STD_LOGIC; -- 本次运算零标志

-- 中断定义
SIGNAL R_REQ: STD_LOGIC; -- 中断请求寄存器
SIGNAL R_ACK: STD_LOGIC; -- 中断响应寄存器
SIGNAL ICEN : STD_LOGIC; -- 取中断指令

-- 24 位微控制信号
SIGNAL S0   : STD_LOGIC;
SIGNAL S1   : STD_LOGIC;
SIGNAL S2   : STD_LOGIC; -- 运算器功能选择
SIGNAL AEN  : STD_LOGIC; -- A写允许
SIGNAL WEN  : STD_LOGIC; -- W写允许
SIGNAL X0   : STD_LOGIC;
SIGNAL X1   : STD_LOGIC;
SIGNAL X2   : STD_LOGIC; -- 寄存器输出控制

SIGNAL FEN  : STD_LOGIC; -- 标志寄存器写允许
SIGNAL CN   : STD_LOGIC; -- 移位时是否带进位
SIGNAL RWR  : STD_LOGIC; -- 寄存器(R0..R3)写允许
SIGNAL RRD  : STD_LOGIC; -- 寄存器(R0..R3)读允许
SIGNAL STEN : STD_LOGIC; -- ST寄存器写允许
SIGNAL OUTEN: STD_LOGIC; -- OUT寄存器写允许
SIGNAL MAROE: STD_LOGIC; -- MAR寄存器地址输出允许
SIGNAL MAREN: STD_LOGIC; -- MAR寄存器写允许

SIGNAL ELP  : STD_LOGIC; -- PC寄存器写允许
SIGNAL EINT : STD_LOGIC; -- 中断结束
SIGNAL IREN : STD_LOGIC; -- IR寄存器写允许
SIGNAL EMEN : STD_LOGIC; -- EM存贮器与数据总线(D_BUS)相通控制位
SIGNAL PCOE : STD_LOGIC; -- PC寄存器地址输出允许
SIGNAL EMRD : STD_LOGIC; -- 主存贮器读允许
SIGNAL EMWR : STD_LOGIC; -- 主存贮器写允许
SIGNAL XRD  : STD_LOGIC; -- 外部I/O读允许

-- ALU运算器定义
SIGNAL T  : STD_LOGIC_VECTOR(8 DOWNTO 0);  -- 运算结果
SIGNAL D  : STD_LOGIC_VECTOR(7 DOWNTO 0);  -- 直通门
SIGNAL R  : STD_LOGIC_VECTOR(7 DOWNTO 0);  -- 右移门
SIGNAL L  : STD_LOGIC_VECTOR(7 DOWNTO 0);  -- 左移门

-- 总线定义
SIGNAL D_BUS: STD_LOGIC_VECTOR(7 DOWNTO 0); -- 数据总线
SIGNAL I_BUS: STD_LOGIC_VECTOR(7 DOWNTO 0); -- 指令总线
SIGNAL A_BUS: STD_LOGIC_VECTOR(7 DOWNTO 0); -- 地址总线

-- 指令周期
SIGNAL RT: STD_LOGIC_VECTOR(1 DOWNTO 0); -- 当前周期数
SIGNAL CT: STD_LOGIC_VECTOR(1 DOWNTO 0); -- 下一条指令总周期数

BEGIN

  IA <= "11100000"; -- 中断向量，定义为 "E0"

  -- 寄存器 A
  PROCESS(clk, rst, AEN)
  BEGIN
    IF rst = '1' THEN
      A <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF AEN = '0' THEN
        A <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 W
  PROCESS(clk, rst, WEN)
  BEGIN
    IF rst = '1' THEN
      W <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF WEN = '0' THEN
        W <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 R0
  PROCESS(clk, rst, RWR, IR)
  BEGIN
    IF rst = '1' THEN
      R0 <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF (RWR = '0') AND (IR(1) = '0') AND (IR(0) = '0') THEN
        R0 <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 R1
  PROCESS(clk, rst, RWR, IR)
  BEGIN
    IF rst = '1' THEN
      R1 <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF (RWR = '0') AND (IR(1) = '0') AND (IR(0) = '1') THEN
        R1 <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 R2
  PROCESS(clk, rst, RWR, IR)
  BEGIN
    IF rst = '1' THEN
      R2 <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF (RWR = '0') AND (IR(1) = '1') AND (IR(0) = '0') THEN
        R2 <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 R3
  PROCESS(clk, rst, RWR, IR)
  BEGIN
    IF rst = '1' THEN
      R3 <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF (RWR = '0') AND (IR(1) = '1') AND (IR(0) = '1') THEN
        R3 <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 OUT
  PROCESS(clk, rst, OUTEN)
  BEGIN
    IF rst = '1' THEN
      portout <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF OUTEN = '0' THEN
        portout <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 ST
  PROCESS(clk, rst, STEN)
  BEGIN
    IF rst = '1' THEN
      ST <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF STEN = '0' THEN
        ST <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 MAR
  PROCESS(clk, rst, MAREN)
  BEGIN
    IF rst = '1' THEN
      MAR <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF MAREN = '0' THEN
        MAR <= D_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 IR
  PROCESS(clk, rst, IREN)
  BEGIN
    IF rst = '1' THEN
      IR <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF IREN = '0' THEN
        IR <= I_BUS;
      END IF;
    END IF;
  END PROCESS;

  -- 寄存器 PC
  PROCESS(clk, rst, IR, R_CY, R_Z, ELP)
  VARIABLE LDPC: STD_LOGIC; -- 转移控制
  BEGIN
    IF rst = '1' THEN
      PC <= "00000000";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF (ELP = '0') AND (
           ( (IR(3) = '0') AND (IR(2) = '0') AND (R_CY = '1') ) OR
           ( (IR(3) = '0') AND (IR(2) = '1') AND (R_Z  = '1') ) OR
           (IR(3) = '1')
         ) THEN LDPC := '0'; ELSE LDPC := '1';
      END IF;

      IF LDPC = '0' THEN
        PC <= D_BUS;
      ELSIF (PCOE = '0') AND (ICEN = '1') THEN  -- 在转入中断时PC不加一
        PC <= PC + 1;
      END IF;
    END IF;
  END PROCESS;

  -- 标志位
  PROCESS(clk, rst, FEN)
  BEGIN
    IF rst = '1' THEN
      R_CY <= '0';
      R_Z  <= '0';
    ELSIF clk'EVENT AND clk = '1' THEN
      IF FEN = '0' THEN
        R_CY <= CY;
        R_Z  <= Z ;
      END IF;
    END IF;
  END PROCESS;

  -- ALU 运算器，第八位为进位标志
  T <= ('0' & A)   + ('0' & W)        WHEN S2 = '0' AND S1 = '0' AND S0 = '0' ELSE
       ('0' & A)   - ('0' & W)        WHEN S2 = '0' AND S1 = '0' AND S0 = '1' ELSE
       ('0' & A)  OR ('0' & W)        WHEN S2 = '0' AND S1 = '1' AND S0 = '0' ELSE
       ('0' & A) AND ('0' & W)        WHEN S2 = '0' AND S1 = '1' AND S0 = '1' ELSE
       ('0' & A)   + ('0' & W) + R_CY WHEN S2 = '1' AND S1 = '0' AND S0 = '0' ELSE
       ('0' & A)   - ('0' & W) - R_CY WHEN S2 = '1' AND S1 = '0' AND S0 = '1' ELSE
       NOT ('0' & A)                  WHEN S2 = '1' AND S1 = '1' AND S0 = '0' ELSE
       '0' & A;

  -- 直通门
  D <= T(7 DOWNTO 0);

  -- 右移门
  R(6 DOWNTO 0) <= T(7 DOWNTO 1);
  R(7)          <= R_CY WHEN CN = '1' ELSE '0';

  -- 左移门
  L(7 DOWNTO 1) <= T(6 DOWNTO 0);
  L(0)          <= R_CY WHEN CN = '1' ELSE '0';

  CY <= T(0) WHEN (X1 = '0') AND (X0 = '1') AND (CN = '1') ELSE      -- 不带进位右移
        R_CY WHEN (X1 = '0') AND (X0 = '1') AND (CN = '0') ELSE      -- 带进位右移
        T(7) WHEN (X1 = '1') AND (X0 = '0') AND (CN = '1') ELSE      -- 不带进位左移
        R_CY WHEN (X1 = '1') AND (X0 = '0') AND (CN = '0') ELSE      -- 带进位左移
        T(8);                                                        -- 直通

  Z  <= '1' WHEN (R = "00000000") AND (X1 = '0') AND (X0 = '1') ELSE -- 右移门
        '1' WHEN (L = "00000000") AND (X1 = '1') AND (X0 = '0') ELSE -- 左移门
        '1' WHEN (D = "00000000") ELSE                               -- 直通门
        '0';

  -- 中断处理
  PROCESS(i_req, rst, EINT)
  BEGIN
    IF (EINT = '0') OR (rst = '1') THEN
      R_REQ <= '0';
    ELSIF i_req'EVENT AND i_req = '1' THEN
      R_REQ <= '1';
    END IF;
  END PROCESS;

  PROCESS(clk, rst, EINT, IREN, R_REQ, R_ACK, ICEN)
  BEGIN
    IF (IREN = '0') AND (R_REQ = '1') AND (R_ACK = '0') THEN
      ICEN <= '0';
    ELSE
      ICEN <= '1';
    END IF;

    IF (EINT = '0') OR (rst = '1') THEN
      R_ACK <= '0';
    ELSIF clk'EVENT AND clk = '1' THEN
      IF ICEN = '0' THEN
        R_ACK <= R_REQ;
      END IF;
    END IF;
  END PROCESS;

  -- 地址总线
  A_BUS <= PC  WHEN PCOE  = '0' ELSE
           MAR; -- WHEN MAROE = '0'

  -- 指令总线
  I_BUS <= "10111000" WHEN ICEN = '0' ELSE mem_d;

  -- 数据总线
  PROCESS(X2, X1, X0, RRD, keyin, IA, ST, PC, D, R, L, R0, R1, R2, R3, IR, I_BUS)
  BEGIN
    IF (X2 = '0') AND (X1 = '0') AND (X0 = '0') THEN
      D_BUS <= keyin;
    ELSIF (X2 = '0') AND (X1 = '0') AND (X0 = '1') THEN
      D_BUS <= IA;
    ELSIF (X2 = '0') AND (X1 = '1') AND (X0 = '0') THEN
      D_BUS <= ST;
    ELSIF (X2 = '0') AND (X1 = '1') AND (X0 = '1') THEN
      D_BUS <= PC;
    ELSIF (X2 = '1') AND (X1 = '0') AND (X0 = '0') THEN
      D_BUS <= D;
    ELSIF (X2 = '1') AND (X1 = '0') AND (X0 = '1') THEN
      D_BUS <= R;
    ELSIF (X2 = '1') AND (X1 = '1') AND (X0 = '0') THEN
      D_BUS <= L;
    ELSIF (RRD = '0') AND (IR(1) = '0') AND (IR(0) = '0') THEN
      D_BUS <= R0;
    ELSIF (RRD = '0') AND (IR(1) = '0') AND (IR(0) = '1') THEN
      D_BUS <= R1;
    ELSIF (RRD = '0') AND (IR(1) = '1') AND (IR(0) = '0') THEN
      D_BUS <= R2;
    ELSIF (RRD = '0') AND (IR(1) = '1') AND (IR(0) = '1') THEN
      D_BUS <= R3;
    ELSE
      D_BUS <= I_BUS; -- (EMRD = '0') AND (EMEN = '0')
    END IF;
  END PROCESS;

  -- 主存贮器EM
  mem_a( 7 DOWNTO 0) <= A_BUS;
  mem_a(14 DOWNTO 8) <= "0000000";
  mem_rd             <= '0'       WHEN (EMRD = '0') AND (ICEN = '1') ELSE '1';
  mem_wr             <= NOT clk   WHEN EMWR = '0' ELSE '1';
  mem_d              <= D_BUS     WHEN EMWR = '0' ELSE "ZZZZZZZZ";

  -- 指令周期
  PROCESS(clk, rst)
  BEGIN
    IF rst = '1' THEN
      RT <= "00";
    ELSIF clk'EVENT AND clk = '1' THEN
      IF RT = "00" THEN
        RT <= CT; -- 取下一条指令时同时算出下一条指令的周期数CT
      ELSE
        RT <= RT - 1;
      END IF;
    END IF;
  END PROCESS;

  -- 24 位微控制信号
  PROCESS(IR, RT)
  VARIABLE STATUS: STD_LOGIC_VECTOR(7 DOWNTO 0);
  BEGIN
    STATUS(7 DOWNTO 2) := IR(7 DOWNTO 2);
    STATUS(1 DOWNTO 0) := RT(1 DOWNTO 0);

    -- XRD 1
    CASE STATUS IS
      WHEN "10010001" => XRD <= '0'; -- READ     A, MM
      WHEN OTHERS     => XRD <= '1';
    END CASE;

    -- EMWR 2
    CASE STATUS IS
      WHEN "10000101" => EMWR <= '0'; -- MOV      @R?, A
      WHEN "10001001" => EMWR <= '0'; -- MOV      MM, A
      WHEN OTHERS     => EMWR <= '1';
    END CASE;

    -- EMRD 100
    CASE STATUS IS
      WHEN "00000000" => EMRD <= '0'; -- _FATCH_  
      WHEN "00000100" => EMRD <= '0'; -- UNDEF
      WHEN "00001000" => EMRD <= '0'; -- UNDEF
      WHEN "00001100" => EMRD <= '0'; -- UNDEF
      WHEN "00010000" => EMRD <= '0'; -- ADD      A, R?
      WHEN "00010110" => EMRD <= '0'; -- ADD      A, @R?
      WHEN "00010100" => EMRD <= '0'; -- ADD      A, @R?
      WHEN "00011011" => EMRD <= '0'; -- ADD      A, MM
      WHEN "00011010" => EMRD <= '0'; -- ADD      A, MM
      WHEN "00011000" => EMRD <= '0'; -- ADD      A, MM
      WHEN "00011110" => EMRD <= '0'; -- ADD      A, #II
      WHEN "00011100" => EMRD <= '0'; -- ADD      A, #II
      WHEN "00100000" => EMRD <= '0'; -- ADDC     A, R?
      WHEN "00100110" => EMRD <= '0'; -- ADDC     A, @R?
      WHEN "00100100" => EMRD <= '0'; -- ADDC     A, @R?
      WHEN "00101011" => EMRD <= '0'; -- ADDC     A, MM
      WHEN "00101010" => EMRD <= '0'; -- ADDC     A, MM
      WHEN "00101000" => EMRD <= '0'; -- ADDC     A, MM
      WHEN "00101110" => EMRD <= '0'; -- ADDC     A, #II
      WHEN "00101100" => EMRD <= '0'; -- ADDC     A, #II
      WHEN "00110000" => EMRD <= '0'; -- SUB      A, R?
      WHEN "00110110" => EMRD <= '0'; -- SUB      A, @R?
      WHEN "00110100" => EMRD <= '0'; -- SUB      A, @R?
      WHEN "00111011" => EMRD <= '0'; -- SUB      A, MM
      WHEN "00111010" => EMRD <= '0'; -- SUB      A, MM
      WHEN "00111000" => EMRD <= '0'; -- SUB      A, MM
      WHEN "00111110" => EMRD <= '0'; -- SUB      A, #II
      WHEN "00111100" => EMRD <= '0'; -- SUB      A, #II
      WHEN "01000000" => EMRD <= '0'; -- SUBC     A, R?
      WHEN "01000110" => EMRD <= '0'; -- SUBC     A, @R?
      WHEN "01000100" => EMRD <= '0'; -- SUBC     A, @R?
      WHEN "01001011" => EMRD <= '0'; -- SUBC     A, MM
      WHEN "01001010" => EMRD <= '0'; -- SUBC     A, MM
      WHEN "01001000" => EMRD <= '0'; -- SUBC     A, MM
      WHEN "01001110" => EMRD <= '0'; -- SUBC     A, #II
      WHEN "01001100" => EMRD <= '0'; -- SUBC     A, #II
      WHEN "01010000" => EMRD <= '0'; -- AND      A, R?
      WHEN "01010110" => EMRD <= '0'; -- AND      A, @R?
      WHEN "01010100" => EMRD <= '0'; -- AND      A, @R?
      WHEN "01011011" => EMRD <= '0'; -- AND      A, MM
      WHEN "01011010" => EMRD <= '0'; -- AND      A, MM
      WHEN "01011000" => EMRD <= '0'; -- AND      A, MM
      WHEN "01011110" => EMRD <= '0'; -- AND      A, #II
      WHEN "01011100" => EMRD <= '0'; -- AND      A, #II
      WHEN "01100000" => EMRD <= '0'; -- OR       A, R?
      WHEN "01100110" => EMRD <= '0'; -- OR       A, @R?
      WHEN "01100100" => EMRD <= '0'; -- OR       A, @R?
      WHEN "01101011" => EMRD <= '0'; -- OR       A, MM
      WHEN "01101010" => EMRD <= '0'; -- OR       A, MM
      WHEN "01101000" => EMRD <= '0'; -- OR       A, MM
      WHEN "01101110" => EMRD <= '0'; -- OR       A, #II
      WHEN "01101100" => EMRD <= '0'; -- OR       A, #II
      WHEN "01110000" => EMRD <= '0'; -- MOV      A, R?
      WHEN "01110101" => EMRD <= '0'; -- MOV      A, @R?
      WHEN "01110100" => EMRD <= '0'; -- MOV      A, @R?
      WHEN "01111010" => EMRD <= '0'; -- MOV      A, MM
      WHEN "01111001" => EMRD <= '0'; -- MOV      A, MM
      WHEN "01111000" => EMRD <= '0'; -- MOV      A, MM
      WHEN "01111101" => EMRD <= '0'; -- MOV      A, #II
      WHEN "01111100" => EMRD <= '0'; -- MOV      A, #II
      WHEN "10000000" => EMRD <= '0'; -- MOV      R?, A
      WHEN "10000100" => EMRD <= '0'; -- MOV      @R?, A
      WHEN "10001010" => EMRD <= '0'; -- MOV      MM, A
      WHEN "10001000" => EMRD <= '0'; -- MOV      MM, A
      WHEN "10001101" => EMRD <= '0'; -- MOV      R?, #II
      WHEN "10001100" => EMRD <= '0'; -- MOV      R?, #II
      WHEN "10010010" => EMRD <= '0'; -- READ     A, MM
      WHEN "10010000" => EMRD <= '0'; -- READ     A, MM
      WHEN "10010110" => EMRD <= '0'; -- WRITE    MM, A
      WHEN "10010100" => EMRD <= '0'; -- WRITE    MM, A
      WHEN "10011000" => EMRD <= '0'; -- UNDEF
      WHEN "10011100" => EMRD <= '0'; -- UNDEF
      WHEN "10100001" => EMRD <= '0'; -- JC       MM
      WHEN "10100000" => EMRD <= '0'; -- JC       MM
      WHEN "10100101" => EMRD <= '0'; -- JZ       MM
      WHEN "10100100" => EMRD <= '0'; -- JZ       MM
      WHEN "10101000" => EMRD <= '0'; -- UNDEF
      WHEN "10101101" => EMRD <= '0'; -- JMP      MM
      WHEN "10101100" => EMRD <= '0'; -- JMP      MM
      WHEN "10110000" => EMRD <= '0'; -- UNDEF
      WHEN "10110100" => EMRD <= '0'; -- UNDEF
      WHEN "10111000" => EMRD <= '0'; -- _INT_    
      WHEN "10111101" => EMRD <= '0'; -- CALL     MM
      WHEN "10111100" => EMRD <= '0'; -- CALL     MM
      WHEN "11000000" => EMRD <= '0'; -- IN       
      WHEN "11000100" => EMRD <= '0'; -- OUT      
      WHEN "11001000" => EMRD <= '0'; -- UNDEF
      WHEN "11001100" => EMRD <= '0'; -- RET      
      WHEN "11010000" => EMRD <= '0'; -- RR       A
      WHEN "11010100" => EMRD <= '0'; -- RL       A
      WHEN "11011000" => EMRD <= '0'; -- RRC      A
      WHEN "11011100" => EMRD <= '0'; -- RLC      A
      WHEN "11100000" => EMRD <= '0'; -- NOP      
      WHEN "11100100" => EMRD <= '0'; -- CPL      A
      WHEN "11101000" => EMRD <= '0'; -- UNDEF
      WHEN "11101100" => EMRD <= '0'; -- RETI     
      WHEN "11110000" => EMRD <= '0'; -- UNDEF
      WHEN "11110100" => EMRD <= '0'; -- UNDEF
      WHEN "11111000" => EMRD <= '0'; -- UNDEF
      WHEN "11111100" => EMRD <= '0'; -- UNDEF
      WHEN OTHERS     => EMRD <= '1';
    END CASE;

    -- PCOE 86
    CASE STATUS IS
      WHEN "00000000" => PCOE <= '0'; -- _FATCH_  
      WHEN "00000100" => PCOE <= '0'; -- UNDEF
      WHEN "00001000" => PCOE <= '0'; -- UNDEF
      WHEN "00001100" => PCOE <= '0'; -- UNDEF
      WHEN "00010000" => PCOE <= '0'; -- ADD      A, R?
      WHEN "00010100" => PCOE <= '0'; -- ADD      A, @R?
      WHEN "00011011" => PCOE <= '0'; -- ADD      A, MM
      WHEN "00011000" => PCOE <= '0'; -- ADD      A, MM
      WHEN "00011110" => PCOE <= '0'; -- ADD      A, #II
      WHEN "00011100" => PCOE <= '0'; -- ADD      A, #II
      WHEN "00100000" => PCOE <= '0'; -- ADDC     A, R?
      WHEN "00100100" => PCOE <= '0'; -- ADDC     A, @R?
      WHEN "00101011" => PCOE <= '0'; -- ADDC     A, MM
      WHEN "00101000" => PCOE <= '0'; -- ADDC     A, MM
      WHEN "00101110" => PCOE <= '0'; -- ADDC     A, #II
      WHEN "00101100" => PCOE <= '0'; -- ADDC     A, #II
      WHEN "00110000" => PCOE <= '0'; -- SUB      A, R?
      WHEN "00110100" => PCOE <= '0'; -- SUB      A, @R?
      WHEN "00111011" => PCOE <= '0'; -- SUB      A, MM
      WHEN "00111000" => PCOE <= '0'; -- SUB      A, MM
      WHEN "00111110" => PCOE <= '0'; -- SUB      A, #II
      WHEN "00111100" => PCOE <= '0'; -- SUB      A, #II
      WHEN "01000000" => PCOE <= '0'; -- SUBC     A, R?
      WHEN "01000100" => PCOE <= '0'; -- SUBC     A, @R?
      WHEN "01001011" => PCOE <= '0'; -- SUBC     A, MM
      WHEN "01001000" => PCOE <= '0'; -- SUBC     A, MM
      WHEN "01001110" => PCOE <= '0'; -- SUBC     A, #II
      WHEN "01001100" => PCOE <= '0'; -- SUBC     A, #II
      WHEN "01010000" => PCOE <= '0'; -- AND      A, R?
      WHEN "01010100" => PCOE <= '0'; -- AND      A, @R?
      WHEN "01011011" => PCOE <= '0'; -- AND      A, MM
      WHEN "01011000" => PCOE <= '0'; -- AND      A, MM
      WHEN "01011110" => PCOE <= '0'; -- AND      A, #II
      WHEN "01011100" => PCOE <= '0'; -- AND      A, #II
      WHEN "01100000" => PCOE <= '0'; -- OR       A, R?
      WHEN "01100100" => PCOE <= '0'; -- OR       A, @R?
      WHEN "01101011" => PCOE <= '0'; -- OR       A, MM
      WHEN "01101000" => PCOE <= '0'; -- OR       A, MM
      WHEN "01101110" => PCOE <= '0'; -- OR       A, #II
      WHEN "01101100" => PCOE <= '0'; -- OR       A, #II
      WHEN "01110000" => PCOE <= '0'; -- MOV      A, R?
      WHEN "01110100" => PCOE <= '0'; -- MOV      A, @R?
      WHEN "01111010" => PCOE <= '0'; -- MOV      A, MM
      WHEN "01111000" => PCOE <= '0'; -- MOV      A, MM
      WHEN "01111101" => PCOE <= '0'; -- MOV      A, #II
      WHEN "01111100" => PCOE <= '0'; -- MOV      A, #II
      WHEN "10000000" => PCOE <= '0'; -- MOV      R?, A
      WHEN "10000100" => PCOE <= '0'; -- MOV      @R?, A
      WHEN "10001010" => PCOE <= '0'; -- MOV      MM, A
      WHEN "10001000" => PCOE <= '0'; -- MOV      MM, A
      WHEN "10001101" => PCOE <= '0'; -- MOV      R?, #II
      WHEN "10001100" => PCOE <= '0'; -- MOV      R?, #II
      WHEN "10010010" => PCOE <= '0'; -- READ     A, MM
      WHEN "10010000" => PCOE <= '0'; -- READ     A, MM
      WHEN "10010110" => PCOE <= '0'; -- WRITE    MM, A
      WHEN "10010100" => PCOE <= '0'; -- WRITE    MM, A
      WHEN "10011000" => PCOE <= '0'; -- UNDEF
      WHEN "10011100" => PCOE <= '0'; -- UNDEF
      WHEN "10100001" => PCOE <= '0'; -- JC       MM
      WHEN "10100000" => PCOE <= '0'; -- JC       MM
      WHEN "10100101" => PCOE <= '0'; -- JZ       MM
      WHEN "10100100" => PCOE <= '0'; -- JZ       MM
      WHEN "10101000" => PCOE <= '0'; -- UNDEF
      WHEN "10101101" => PCOE <= '0'; -- JMP      MM
      WHEN "10101100" => PCOE <= '0'; -- JMP      MM
      WHEN "10110000" => PCOE <= '0'; -- UNDEF
      WHEN "10110100" => PCOE <= '0'; -- UNDEF
      WHEN "10111000" => PCOE <= '0'; -- _INT_    
      WHEN "10111111" => PCOE <= '0'; -- CALL     MM
      WHEN "10111100" => PCOE <= '0'; -- CALL     MM
      WHEN "11000000" => PCOE <= '0'; -- IN       
      WHEN "11000100" => PCOE <= '0'; -- OUT      
      WHEN "11001000" => PCOE <= '0'; -- UNDEF
      WHEN "11001100" => PCOE <= '0'; -- RET      
      WHEN "11010000" => PCOE <= '0'; -- RR       A
      WHEN "11010100" => PCOE <= '0'; -- RL       A
      WHEN "11011000" => PCOE <= '0'; -- RRC      A
      WHEN "11011100" => PCOE <= '0'; -- RLC      A
      WHEN "11100000" => PCOE <= '0'; -- NOP      
      WHEN "11100100" => PCOE <= '0'; -- CPL      A
      WHEN "11101000" => PCOE <= '0'; -- UNDEF
      WHEN "11101100" => PCOE <= '0'; -- RETI     
      WHEN "11110000" => PCOE <= '0'; -- UNDEF
      WHEN "11110100" => PCOE <= '0'; -- UNDEF
      WHEN "11111000" => PCOE <= '0'; -- UNDEF
      WHEN "11111100" => PCOE <= '0'; -- UNDEF
      WHEN OTHERS     => PCOE <= '1';
    END CASE;

    -- EMEN 38
    CASE STATUS IS
      WHEN "00010110" => EMEN <= '0'; -- ADD      A, @R?
      WHEN "00011011" => EMEN <= '0'; -- ADD      A, MM
      WHEN "00011010" => EMEN <= '0'; -- ADD      A, MM
      WHEN "00011110" => EMEN <= '0'; -- ADD      A, #II
      WHEN "00100110" => EMEN <= '0'; -- ADDC     A, @R?
      WHEN "00101011" => EMEN <= '0'; -- ADDC     A, MM
      WHEN "00101010" => EMEN <= '0'; -- ADDC     A, MM
      WHEN "00101110" => EMEN <= '0'; -- ADDC     A, #II
      WHEN "00110110" => EMEN <= '0'; -- SUB      A, @R?
      WHEN "00111011" => EMEN <= '0'; -- SUB      A, MM
      WHEN "00111010" => EMEN <= '0'; -- SUB      A, MM
      WHEN "00111110" => EMEN <= '0'; -- SUB      A, #II
      WHEN "01000110" => EMEN <= '0'; -- SUBC     A, @R?
      WHEN "01001011" => EMEN <= '0'; -- SUBC     A, MM
      WHEN "01001010" => EMEN <= '0'; -- SUBC     A, MM
      WHEN "01001110" => EMEN <= '0'; -- SUBC     A, #II
      WHEN "01010110" => EMEN <= '0'; -- AND      A, @R?
      WHEN "01011011" => EMEN <= '0'; -- AND      A, MM
      WHEN "01011010" => EMEN <= '0'; -- AND      A, MM
      WHEN "01011110" => EMEN <= '0'; -- AND      A, #II
      WHEN "01100110" => EMEN <= '0'; -- OR       A, @R?
      WHEN "01101011" => EMEN <= '0'; -- OR       A, MM
      WHEN "01101010" => EMEN <= '0'; -- OR       A, MM
      WHEN "01101110" => EMEN <= '0'; -- OR       A, #II
      WHEN "01110101" => EMEN <= '0'; -- MOV      A, @R?
      WHEN "01111010" => EMEN <= '0'; -- MOV      A, MM
      WHEN "01111001" => EMEN <= '0'; -- MOV      A, MM
      WHEN "01111101" => EMEN <= '0'; -- MOV      A, #II
      WHEN "10000101" => EMEN <= '0'; -- MOV      @R?, A
      WHEN "10001010" => EMEN <= '0'; -- MOV      MM, A
      WHEN "10001001" => EMEN <= '0'; -- MOV      MM, A
      WHEN "10001101" => EMEN <= '0'; -- MOV      R?, #II
      WHEN "10010010" => EMEN <= '0'; -- READ     A, MM
      WHEN "10010110" => EMEN <= '0'; -- WRITE    MM, A
      WHEN "10100001" => EMEN <= '0'; -- JC       MM
      WHEN "10100101" => EMEN <= '0'; -- JZ       MM
      WHEN "10101101" => EMEN <= '0'; -- JMP      MM
      WHEN "10111101" => EMEN <= '0'; -- CALL     MM
      WHEN OTHERS     => EMEN <= '1';
    END CASE;

    -- IREN 64
    CASE STATUS IS
      WHEN "00000000" => IREN <= '0'; -- _FATCH_  
      WHEN "00000100" => IREN <= '0'; -- UNDEF
      WHEN "00001000" => IREN <= '0'; -- UNDEF
      WHEN "00001100" => IREN <= '0'; -- UNDEF
      WHEN "00010000" => IREN <= '0'; -- ADD      A, R?
      WHEN "00010100" => IREN <= '0'; -- ADD      A, @R?
      WHEN "00011000" => IREN <= '0'; -- ADD      A, MM
      WHEN "00011100" => IREN <= '0'; -- ADD      A, #II
      WHEN "00100000" => IREN <= '0'; -- ADDC     A, R?
      WHEN "00100100" => IREN <= '0'; -- ADDC     A, @R?
      WHEN "00101000" => IREN <= '0'; -- ADDC     A, MM
      WHEN "00101100" => IREN <= '0'; -- ADDC     A, #II
      WHEN "00110000" => IREN <= '0'; -- SUB      A, R?
      WHEN "00110100" => IREN <= '0'; -- SUB      A, @R?
      WHEN "00111000" => IREN <= '0'; -- SUB      A, MM
      WHEN "00111100" => IREN <= '0'; -- SUB      A, #II
      WHEN "01000000" => IREN <= '0'; -- SUBC     A, R?
      WHEN "01000100" => IREN <= '0'; -- SUBC     A, @R?
      WHEN "01001000" => IREN <= '0'; -- SUBC     A, MM
      WHEN "01001100" => IREN <= '0'; -- SUBC     A, #II
      WHEN "01010000" => IREN <= '0'; -- AND      A, R?
      WHEN "01010100" => IREN <= '0'; -- AND      A, @R?
      WHEN "01011000" => IREN <= '0'; -- AND      A, MM
      WHEN "01011100" => IREN <= '0'; -- AND      A, #II
      WHEN "01100000" => IREN <= '0'; -- OR       A, R?
      WHEN "01100100" => IREN <= '0'; -- OR       A, @R?
      WHEN "01101000" => IREN <= '0'; -- OR       A, MM
      WHEN "01101100" => IREN <= '0'; -- OR       A, #II
      WHEN "01110000" => IREN <= '0'; -- MOV      A, R?
      WHEN "01110100" => IREN <= '0'; -- MOV      A, @R?
      WHEN "01111000" => IREN <= '0'; -- MOV      A, MM
      WHEN "01111100" => IREN <= '0'; -- MOV      A, #II
      WHEN "10000000" => IREN <= '0'; -- MOV      R?, A
      WHEN "10000100" => IREN <= '0'; -- MOV      @R?, A
      WHEN "10001000" => IREN <= '0'; -- MOV      MM, A
      WHEN "10001100" => IREN <= '0'; -- MOV      R?, #II
      WHEN "10010000" => IREN <= '0'; -- READ     A, MM
      WHEN "10010100" => IREN <= '0'; -- WRITE    MM, A
      WHEN "10011000" => IREN <= '0'; -- UNDEF
      WHEN "10011100" => IREN <= '0'; -- UNDEF
      WHEN "10100000" => IREN <= '0'; -- JC       MM
      WHEN "10100100" => IREN <= '0'; -- JZ       MM
      WHEN "10101000" => IREN <= '0'; -- UNDEF
      WHEN "10101100" => IREN <= '0'; -- JMP      MM
      WHEN "10110000" => IREN <= '0'; -- UNDEF
      WHEN "10110100" => IREN <= '0'; -- UNDEF
      WHEN "10111000" => IREN <= '0'; -- _INT_    
      WHEN "10111100" => IREN <= '0'; -- CALL     MM
      WHEN "11000000" => IREN <= '0'; -- IN       
      WHEN "11000100" => IREN <= '0'; -- OUT      
      WHEN "11001000" => IREN <= '0'; -- UNDEF
      WHEN "11001100" => IREN <= '0'; -- RET      
      WHEN "11010000" => IREN <= '0'; -- RR       A
      WHEN "11010100" => IREN <= '0'; -- RL       A
      WHEN "11011000" => IREN <= '0'; -- RRC      A
      WHEN "11011100" => IREN <= '0'; -- RLC      A
      WHEN "11100000" => IREN <= '0'; -- NOP      
      WHEN "11100100" => IREN <= '0'; -- CPL      A
      WHEN "11101000" => IREN <= '0'; -- UNDEF
      WHEN "11101100" => IREN <= '0'; -- RETI     
      WHEN "11110000" => IREN <= '0'; -- UNDEF
      WHEN "11110100" => IREN <= '0'; -- UNDEF
      WHEN "11111000" => IREN <= '0'; -- UNDEF
      WHEN "11111100" => IREN <= '0'; -- UNDEF
      WHEN OTHERS     => IREN <= '1';
    END CASE;

    -- EINT 1
    CASE STATUS IS
      WHEN "11101101" => EINT <= '0'; -- RETI     
      WHEN OTHERS     => EINT <= '1';
    END CASE;

    -- ELP 7
    CASE STATUS IS
      WHEN "10100001" => ELP <= '0'; -- JC       MM
      WHEN "10100101" => ELP <= '0'; -- JZ       MM
      WHEN "10101101" => ELP <= '0'; -- JMP      MM
      WHEN "10111001" => ELP <= '0'; -- _INT_    
      WHEN "10111101" => ELP <= '0'; -- CALL     MM
      WHEN "11001101" => ELP <= '0'; -- RET      
      WHEN "11101101" => ELP <= '0'; -- RETI     
      WHEN OTHERS     => ELP <= '1';
    END CASE;

    -- MAREN 19
    CASE STATUS IS
      WHEN "00010111" => MAREN <= '0'; -- ADD      A, @R?
      WHEN "00011011" => MAREN <= '0'; -- ADD      A, MM
      WHEN "00100111" => MAREN <= '0'; -- ADDC     A, @R?
      WHEN "00101011" => MAREN <= '0'; -- ADDC     A, MM
      WHEN "00110111" => MAREN <= '0'; -- SUB      A, @R?
      WHEN "00111011" => MAREN <= '0'; -- SUB      A, MM
      WHEN "01000111" => MAREN <= '0'; -- SUBC     A, @R?
      WHEN "01001011" => MAREN <= '0'; -- SUBC     A, MM
      WHEN "01010111" => MAREN <= '0'; -- AND      A, @R?
      WHEN "01011011" => MAREN <= '0'; -- AND      A, MM
      WHEN "01100111" => MAREN <= '0'; -- OR       A, @R?
      WHEN "01101011" => MAREN <= '0'; -- OR       A, MM
      WHEN "01110110" => MAREN <= '0'; -- MOV      A, @R?
      WHEN "01111010" => MAREN <= '0'; -- MOV      A, MM
      WHEN "10000110" => MAREN <= '0'; -- MOV      @R?, A
      WHEN "10001010" => MAREN <= '0'; -- MOV      MM, A
      WHEN "10010010" => MAREN <= '0'; -- READ     A, MM
      WHEN "10010110" => MAREN <= '0'; -- WRITE    MM, A
      WHEN "10111111" => MAREN <= '0'; -- CALL     MM
      WHEN OTHERS     => MAREN <= '1';
    END CASE;

    -- MAROE 19
    CASE STATUS IS
      WHEN "00010110" => MAROE <= '0'; -- ADD      A, @R?
      WHEN "00011010" => MAROE <= '0'; -- ADD      A, MM
      WHEN "00100110" => MAROE <= '0'; -- ADDC     A, @R?
      WHEN "00101010" => MAROE <= '0'; -- ADDC     A, MM
      WHEN "00110110" => MAROE <= '0'; -- SUB      A, @R?
      WHEN "00111010" => MAROE <= '0'; -- SUB      A, MM
      WHEN "01000110" => MAROE <= '0'; -- SUBC     A, @R?
      WHEN "01001010" => MAROE <= '0'; -- SUBC     A, MM
      WHEN "01010110" => MAROE <= '0'; -- AND      A, @R?
      WHEN "01011010" => MAROE <= '0'; -- AND      A, MM
      WHEN "01100110" => MAROE <= '0'; -- OR       A, @R?
      WHEN "01101010" => MAROE <= '0'; -- OR       A, MM
      WHEN "01110101" => MAROE <= '0'; -- MOV      A, @R?
      WHEN "01111001" => MAROE <= '0'; -- MOV      A, MM
      WHEN "10000101" => MAROE <= '0'; -- MOV      @R?, A
      WHEN "10001001" => MAROE <= '0'; -- MOV      MM, A
      WHEN "10010001" => MAROE <= '0'; -- READ     A, MM
      WHEN "10010101" => MAROE <= '0'; -- WRITE    MM, A
      WHEN "10111101" => MAROE <= '0'; -- CALL     MM
      WHEN OTHERS     => MAROE <= '1';
    END CASE;

    -- OUTEN 2
    CASE STATUS IS
      WHEN "10010101" => OUTEN <= '0'; -- WRITE    MM, A
      WHEN "11000101" => OUTEN <= '0'; -- OUT      
      WHEN OTHERS     => OUTEN <= '1';
    END CASE;

    -- STEN 2
    CASE STATUS IS
      WHEN "10111010" => STEN <= '0'; -- _INT_    
      WHEN "10111110" => STEN <= '0'; -- CALL     MM
      WHEN OTHERS     => STEN <= '1';
    END CASE;

    -- RRD 15
    CASE STATUS IS
      WHEN "00010010" => RRD <= '0'; -- ADD      A, R?
      WHEN "00010111" => RRD <= '0'; -- ADD      A, @R?
      WHEN "00100010" => RRD <= '0'; -- ADDC     A, R?
      WHEN "00100111" => RRD <= '0'; -- ADDC     A, @R?
      WHEN "00110010" => RRD <= '0'; -- SUB      A, R?
      WHEN "00110111" => RRD <= '0'; -- SUB      A, @R?
      WHEN "01000010" => RRD <= '0'; -- SUBC     A, R?
      WHEN "01000111" => RRD <= '0'; -- SUBC     A, @R?
      WHEN "01010010" => RRD <= '0'; -- AND      A, R?
      WHEN "01010111" => RRD <= '0'; -- AND      A, @R?
      WHEN "01100010" => RRD <= '0'; -- OR       A, R?
      WHEN "01100111" => RRD <= '0'; -- OR       A, @R?
      WHEN "01110001" => RRD <= '0'; -- MOV      A, R?
      WHEN "01110110" => RRD <= '0'; -- MOV      A, @R?
      WHEN "10000110" => RRD <= '0'; -- MOV      @R?, A
      WHEN OTHERS     => RRD <= '1';
    END CASE;

    -- RWR 2
    CASE STATUS IS
      WHEN "10000001" => RWR <= '0'; -- MOV      R?, A
      WHEN "10001101" => RWR <= '0'; -- MOV      R?, #II
      WHEN OTHERS     => RWR <= '1';
    END CASE;

    -- CN 2
    CASE STATUS IS
      WHEN "11010001" => CN <= '0'; -- RR       A
      WHEN "11010101" => CN <= '0'; -- RL       A
      WHEN OTHERS     => CN <= '1';
    END CASE;

    -- FEN 29
    CASE STATUS IS
      WHEN "00010001" => FEN <= '0'; -- ADD      A, R?
      WHEN "00010101" => FEN <= '0'; -- ADD      A, @R?
      WHEN "00011001" => FEN <= '0'; -- ADD      A, MM
      WHEN "00011101" => FEN <= '0'; -- ADD      A, #II
      WHEN "00100001" => FEN <= '0'; -- ADDC     A, R?
      WHEN "00100101" => FEN <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => FEN <= '0'; -- ADDC     A, MM
      WHEN "00101101" => FEN <= '0'; -- ADDC     A, #II
      WHEN "00110001" => FEN <= '0'; -- SUB      A, R?
      WHEN "00110101" => FEN <= '0'; -- SUB      A, @R?
      WHEN "00111001" => FEN <= '0'; -- SUB      A, MM
      WHEN "00111101" => FEN <= '0'; -- SUB      A, #II
      WHEN "01000001" => FEN <= '0'; -- SUBC     A, R?
      WHEN "01000101" => FEN <= '0'; -- SUBC     A, @R?
      WHEN "01001001" => FEN <= '0'; -- SUBC     A, MM
      WHEN "01001101" => FEN <= '0'; -- SUBC     A, #II
      WHEN "01010001" => FEN <= '0'; -- AND      A, R?
      WHEN "01010101" => FEN <= '0'; -- AND      A, @R?
      WHEN "01011001" => FEN <= '0'; -- AND      A, MM
      WHEN "01011101" => FEN <= '0'; -- AND      A, #II
      WHEN "01100001" => FEN <= '0'; -- OR       A, R?
      WHEN "01100101" => FEN <= '0'; -- OR       A, @R?
      WHEN "01101001" => FEN <= '0'; -- OR       A, MM
      WHEN "01101101" => FEN <= '0'; -- OR       A, #II
      WHEN "11010001" => FEN <= '0'; -- RR       A
      WHEN "11010101" => FEN <= '0'; -- RL       A
      WHEN "11011001" => FEN <= '0'; -- RRC      A
      WHEN "11011101" => FEN <= '0'; -- RLC      A
      WHEN "11100101" => FEN <= '0'; -- CPL      A
      WHEN OTHERS     => FEN <= '1';
    END CASE;

    -- X2 7
    CASE STATUS IS
      WHEN "10111010" => X2 <= '0'; -- _INT_    
      WHEN "10111001" => X2 <= '0'; -- _INT_    
      WHEN "10111111" => X2 <= '0'; -- CALL     MM
      WHEN "10111110" => X2 <= '0'; -- CALL     MM
      WHEN "11000001" => X2 <= '0'; -- IN       
      WHEN "11001101" => X2 <= '0'; -- RET      
      WHEN "11101101" => X2 <= '0'; -- RETI     
      WHEN OTHERS     => X2 <= '1';
    END CASE;

    -- X1 34
    CASE STATUS IS
      WHEN "00010001" => X1 <= '0'; -- ADD      A, R?
      WHEN "00010101" => X1 <= '0'; -- ADD      A, @R?
      WHEN "00011001" => X1 <= '0'; -- ADD      A, MM
      WHEN "00011101" => X1 <= '0'; -- ADD      A, #II
      WHEN "00100001" => X1 <= '0'; -- ADDC     A, R?
      WHEN "00100101" => X1 <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => X1 <= '0'; -- ADDC     A, MM
      WHEN "00101101" => X1 <= '0'; -- ADDC     A, #II
      WHEN "00110001" => X1 <= '0'; -- SUB      A, R?
      WHEN "00110101" => X1 <= '0'; -- SUB      A, @R?
      WHEN "00111001" => X1 <= '0'; -- SUB      A, MM
      WHEN "00111101" => X1 <= '0'; -- SUB      A, #II
      WHEN "01000001" => X1 <= '0'; -- SUBC     A, R?
      WHEN "01000101" => X1 <= '0'; -- SUBC     A, @R?
      WHEN "01001001" => X1 <= '0'; -- SUBC     A, MM
      WHEN "01001101" => X1 <= '0'; -- SUBC     A, #II
      WHEN "01010001" => X1 <= '0'; -- AND      A, R?
      WHEN "01010101" => X1 <= '0'; -- AND      A, @R?
      WHEN "01011001" => X1 <= '0'; -- AND      A, MM
      WHEN "01011101" => X1 <= '0'; -- AND      A, #II
      WHEN "01100001" => X1 <= '0'; -- OR       A, R?
      WHEN "01100101" => X1 <= '0'; -- OR       A, @R?
      WHEN "01101001" => X1 <= '0'; -- OR       A, MM
      WHEN "01101101" => X1 <= '0'; -- OR       A, #II
      WHEN "10000001" => X1 <= '0'; -- MOV      R?, A
      WHEN "10000101" => X1 <= '0'; -- MOV      @R?, A
      WHEN "10001001" => X1 <= '0'; -- MOV      MM, A
      WHEN "10010101" => X1 <= '0'; -- WRITE    MM, A
      WHEN "10111001" => X1 <= '0'; -- _INT_    
      WHEN "11000001" => X1 <= '0'; -- IN       
      WHEN "11000101" => X1 <= '0'; -- OUT      
      WHEN "11010001" => X1 <= '0'; -- RR       A
      WHEN "11011001" => X1 <= '0'; -- RRC      A
      WHEN "11100101" => X1 <= '0'; -- CPL      A
      WHEN OTHERS     => X1 <= '1';
    END CASE;

    -- X0 35
    CASE STATUS IS
      WHEN "00010001" => X0 <= '0'; -- ADD      A, R?
      WHEN "00010101" => X0 <= '0'; -- ADD      A, @R?
      WHEN "00011001" => X0 <= '0'; -- ADD      A, MM
      WHEN "00011101" => X0 <= '0'; -- ADD      A, #II
      WHEN "00100001" => X0 <= '0'; -- ADDC     A, R?
      WHEN "00100101" => X0 <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => X0 <= '0'; -- ADDC     A, MM
      WHEN "00101101" => X0 <= '0'; -- ADDC     A, #II
      WHEN "00110001" => X0 <= '0'; -- SUB      A, R?
      WHEN "00110101" => X0 <= '0'; -- SUB      A, @R?
      WHEN "00111001" => X0 <= '0'; -- SUB      A, MM
      WHEN "00111101" => X0 <= '0'; -- SUB      A, #II
      WHEN "01000001" => X0 <= '0'; -- SUBC     A, R?
      WHEN "01000101" => X0 <= '0'; -- SUBC     A, @R?
      WHEN "01001001" => X0 <= '0'; -- SUBC     A, MM
      WHEN "01001101" => X0 <= '0'; -- SUBC     A, #II
      WHEN "01010001" => X0 <= '0'; -- AND      A, R?
      WHEN "01010101" => X0 <= '0'; -- AND      A, @R?
      WHEN "01011001" => X0 <= '0'; -- AND      A, MM
      WHEN "01011101" => X0 <= '0'; -- AND      A, #II
      WHEN "01100001" => X0 <= '0'; -- OR       A, R?
      WHEN "01100101" => X0 <= '0'; -- OR       A, @R?
      WHEN "01101001" => X0 <= '0'; -- OR       A, MM
      WHEN "01101101" => X0 <= '0'; -- OR       A, #II
      WHEN "10000001" => X0 <= '0'; -- MOV      R?, A
      WHEN "10000101" => X0 <= '0'; -- MOV      @R?, A
      WHEN "10001001" => X0 <= '0'; -- MOV      MM, A
      WHEN "10010101" => X0 <= '0'; -- WRITE    MM, A
      WHEN "11000001" => X0 <= '0'; -- IN       
      WHEN "11000101" => X0 <= '0'; -- OUT      
      WHEN "11001101" => X0 <= '0'; -- RET      
      WHEN "11010101" => X0 <= '0'; -- RL       A
      WHEN "11011101" => X0 <= '0'; -- RLC      A
      WHEN "11100101" => X0 <= '0'; -- CPL      A
      WHEN "11101101" => X0 <= '0'; -- RETI     
      WHEN OTHERS     => X0 <= '1';
    END CASE;

    -- WEN 24
    CASE STATUS IS
      WHEN "00010010" => WEN <= '0'; -- ADD      A, R?
      WHEN "00010110" => WEN <= '0'; -- ADD      A, @R?
      WHEN "00011010" => WEN <= '0'; -- ADD      A, MM
      WHEN "00011110" => WEN <= '0'; -- ADD      A, #II
      WHEN "00100010" => WEN <= '0'; -- ADDC     A, R?
      WHEN "00100110" => WEN <= '0'; -- ADDC     A, @R?
      WHEN "00101010" => WEN <= '0'; -- ADDC     A, MM
      WHEN "00101110" => WEN <= '0'; -- ADDC     A, #II
      WHEN "00110010" => WEN <= '0'; -- SUB      A, R?
      WHEN "00110110" => WEN <= '0'; -- SUB      A, @R?
      WHEN "00111010" => WEN <= '0'; -- SUB      A, MM
      WHEN "00111110" => WEN <= '0'; -- SUB      A, #II
      WHEN "01000010" => WEN <= '0'; -- SUBC     A, R?
      WHEN "01000110" => WEN <= '0'; -- SUBC     A, @R?
      WHEN "01001010" => WEN <= '0'; -- SUBC     A, MM
      WHEN "01001110" => WEN <= '0'; -- SUBC     A, #II
      WHEN "01010010" => WEN <= '0'; -- AND      A, R?
      WHEN "01010110" => WEN <= '0'; -- AND      A, @R?
      WHEN "01011010" => WEN <= '0'; -- AND      A, MM
      WHEN "01011110" => WEN <= '0'; -- AND      A, #II
      WHEN "01100010" => WEN <= '0'; -- OR       A, R?
      WHEN "01100110" => WEN <= '0'; -- OR       A, @R?
      WHEN "01101010" => WEN <= '0'; -- OR       A, MM
      WHEN "01101110" => WEN <= '0'; -- OR       A, #II
      WHEN OTHERS     => WEN <= '1';
    END CASE;

    -- AEN 35
    CASE STATUS IS
      WHEN "00010001" => AEN <= '0'; -- ADD      A, R?
      WHEN "00010101" => AEN <= '0'; -- ADD      A, @R?
      WHEN "00011001" => AEN <= '0'; -- ADD      A, MM
      WHEN "00011101" => AEN <= '0'; -- ADD      A, #II
      WHEN "00100001" => AEN <= '0'; -- ADDC     A, R?
      WHEN "00100101" => AEN <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => AEN <= '0'; -- ADDC     A, MM
      WHEN "00101101" => AEN <= '0'; -- ADDC     A, #II
      WHEN "00110001" => AEN <= '0'; -- SUB      A, R?
      WHEN "00110101" => AEN <= '0'; -- SUB      A, @R?
      WHEN "00111001" => AEN <= '0'; -- SUB      A, MM
      WHEN "00111101" => AEN <= '0'; -- SUB      A, #II
      WHEN "01000001" => AEN <= '0'; -- SUBC     A, R?
      WHEN "01000101" => AEN <= '0'; -- SUBC     A, @R?
      WHEN "01001001" => AEN <= '0'; -- SUBC     A, MM
      WHEN "01001101" => AEN <= '0'; -- SUBC     A, #II
      WHEN "01010001" => AEN <= '0'; -- AND      A, R?
      WHEN "01010101" => AEN <= '0'; -- AND      A, @R?
      WHEN "01011001" => AEN <= '0'; -- AND      A, MM
      WHEN "01011101" => AEN <= '0'; -- AND      A, #II
      WHEN "01100001" => AEN <= '0'; -- OR       A, R?
      WHEN "01100101" => AEN <= '0'; -- OR       A, @R?
      WHEN "01101001" => AEN <= '0'; -- OR       A, MM
      WHEN "01101101" => AEN <= '0'; -- OR       A, #II
      WHEN "01110001" => AEN <= '0'; -- MOV      A, R?
      WHEN "01110101" => AEN <= '0'; -- MOV      A, @R?
      WHEN "01111001" => AEN <= '0'; -- MOV      A, MM
      WHEN "01111101" => AEN <= '0'; -- MOV      A, #II
      WHEN "10010001" => AEN <= '0'; -- READ     A, MM
      WHEN "11000001" => AEN <= '0'; -- IN       
      WHEN "11010001" => AEN <= '0'; -- RR       A
      WHEN "11010101" => AEN <= '0'; -- RL       A
      WHEN "11011001" => AEN <= '0'; -- RRC      A
      WHEN "11011101" => AEN <= '0'; -- RLC      A
      WHEN "11100101" => AEN <= '0'; -- CPL      A
      WHEN OTHERS     => AEN <= '1';
    END CASE;

    -- S2 16
    CASE STATUS IS
      WHEN "00010001" => S2 <= '0'; -- ADD      A, R?
      WHEN "00010101" => S2 <= '0'; -- ADD      A, @R?
      WHEN "00011001" => S2 <= '0'; -- ADD      A, MM
      WHEN "00011101" => S2 <= '0'; -- ADD      A, #II
      WHEN "00110001" => S2 <= '0'; -- SUB      A, R?
      WHEN "00110101" => S2 <= '0'; -- SUB      A, @R?
      WHEN "00111001" => S2 <= '0'; -- SUB      A, MM
      WHEN "00111101" => S2 <= '0'; -- SUB      A, #II
      WHEN "01010001" => S2 <= '0'; -- AND      A, R?
      WHEN "01010101" => S2 <= '0'; -- AND      A, @R?
      WHEN "01011001" => S2 <= '0'; -- AND      A, MM
      WHEN "01011101" => S2 <= '0'; -- AND      A, #II
      WHEN "01100001" => S2 <= '0'; -- OR       A, R?
      WHEN "01100101" => S2 <= '0'; -- OR       A, @R?
      WHEN "01101001" => S2 <= '0'; -- OR       A, MM
      WHEN "01101101" => S2 <= '0'; -- OR       A, #II
      WHEN OTHERS     => S2 <= '1';
    END CASE;

    -- S1 16
    CASE STATUS IS
      WHEN "00010001" => S1 <= '0'; -- ADD      A, R?
      WHEN "00010101" => S1 <= '0'; -- ADD      A, @R?
      WHEN "00011001" => S1 <= '0'; -- ADD      A, MM
      WHEN "00011101" => S1 <= '0'; -- ADD      A, #II
      WHEN "00100001" => S1 <= '0'; -- ADDC     A, R?
      WHEN "00100101" => S1 <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => S1 <= '0'; -- ADDC     A, MM
      WHEN "00101101" => S1 <= '0'; -- ADDC     A, #II
      WHEN "00110001" => S1 <= '0'; -- SUB      A, R?
      WHEN "00110101" => S1 <= '0'; -- SUB      A, @R?
      WHEN "00111001" => S1 <= '0'; -- SUB      A, MM
      WHEN "00111101" => S1 <= '0'; -- SUB      A, #II
      WHEN "01000001" => S1 <= '0'; -- SUBC     A, R?
      WHEN "01000101" => S1 <= '0'; -- SUBC     A, @R?
      WHEN "01001001" => S1 <= '0'; -- SUBC     A, MM
      WHEN "01001101" => S1 <= '0'; -- SUBC     A, #II
      WHEN OTHERS     => S1 <= '1';
    END CASE;

    -- S0 13
    CASE STATUS IS
      WHEN "00010001" => S0 <= '0'; -- ADD      A, R?
      WHEN "00010101" => S0 <= '0'; -- ADD      A, @R?
      WHEN "00011001" => S0 <= '0'; -- ADD      A, MM
      WHEN "00011101" => S0 <= '0'; -- ADD      A, #II
      WHEN "00100001" => S0 <= '0'; -- ADDC     A, R?
      WHEN "00100101" => S0 <= '0'; -- ADDC     A, @R?
      WHEN "00101001" => S0 <= '0'; -- ADDC     A, MM
      WHEN "00101101" => S0 <= '0'; -- ADDC     A, #II
      WHEN "01100001" => S0 <= '0'; -- OR       A, R?
      WHEN "01100101" => S0 <= '0'; -- OR       A, @R?
      WHEN "01101001" => S0 <= '0'; -- OR       A, MM
      WHEN "01101101" => S0 <= '0'; -- OR       A, #II
      WHEN "11100101" => S0 <= '0'; -- CPL      A
      WHEN OTHERS     => S0 <= '1';
    END CASE;
  END PROCESS;

  -- 计算指令周期数
  PROCESS(I_BUS)
  BEGIN
    CASE I_BUS(7 DOWNTO 2) IS
      WHEN "000100" => CT <= "10";  -- ADD      A, R?
      WHEN "000101" => CT <= "11";  -- ADD      A, @R?
      WHEN "000110" => CT <= "11";  -- ADD      A, MM
      WHEN "000111" => CT <= "10";  -- ADD      A, #II
      WHEN "001000" => CT <= "10";  -- ADDC     A, R?
      WHEN "001001" => CT <= "11";  -- ADDC     A, @R?
      WHEN "001010" => CT <= "11";  -- ADDC     A, MM
      WHEN "001011" => CT <= "10";  -- ADDC     A, #II
      WHEN "001100" => CT <= "10";  -- SUB      A, R?
      WHEN "001101" => CT <= "11";  -- SUB      A, @R?
      WHEN "001110" => CT <= "11";  -- SUB      A, MM
      WHEN "001111" => CT <= "10";  -- SUB      A, #II
      WHEN "010000" => CT <= "10";  -- SUBC     A, R?
      WHEN "010001" => CT <= "11";  -- SUBC     A, @R?
      WHEN "010010" => CT <= "11";  -- SUBC     A, MM
      WHEN "010011" => CT <= "10";  -- SUBC     A, #II
      WHEN "010100" => CT <= "10";  -- AND      A, R?
      WHEN "010101" => CT <= "11";  -- AND      A, @R?
      WHEN "010110" => CT <= "11";  -- AND      A, MM
      WHEN "010111" => CT <= "10";  -- AND      A, #II
      WHEN "011000" => CT <= "10";  -- OR       A, R?
      WHEN "011001" => CT <= "11";  -- OR       A, @R?
      WHEN "011010" => CT <= "11";  -- OR       A, MM
      WHEN "011011" => CT <= "10";  -- OR       A, #II
      WHEN "011100" => CT <= "01";  -- MOV      A, R?
      WHEN "011101" => CT <= "10";  -- MOV      A, @R?
      WHEN "011110" => CT <= "10";  -- MOV      A, MM
      WHEN "011111" => CT <= "01";  -- MOV      A, #II
      WHEN "100000" => CT <= "01";  -- MOV      R?, A
      WHEN "100001" => CT <= "10";  -- MOV      @R?, A
      WHEN "100010" => CT <= "10";  -- MOV      MM, A
      WHEN "100011" => CT <= "01";  -- MOV      R?, #II
      WHEN "100100" => CT <= "10";  -- READ     A, MM
      WHEN "100101" => CT <= "10";  -- WRITE    MM, A
      WHEN "101000" => CT <= "01";  -- JC       MM
      WHEN "101001" => CT <= "01";  -- JZ       MM
      WHEN "101011" => CT <= "01";  -- JMP      MM
      WHEN "101110" => CT <= "10";  -- _INT_    
      WHEN "101111" => CT <= "11";  -- CALL     MM
      WHEN "110000" => CT <= "01";  -- IN       
      WHEN "110001" => CT <= "01";  -- OUT      
      WHEN "110011" => CT <= "01";  -- RET      
      WHEN "110100" => CT <= "01";  -- RR       A
      WHEN "110101" => CT <= "01";  -- RL       A
      WHEN "110110" => CT <= "01";  -- RRC      A
      WHEN "110111" => CT <= "01";  -- RLC      A
      WHEN "111001" => CT <= "01";  -- CPL      A
      WHEN "111011" => CT <= "01";  -- RETI     
      WHEN OTHERS   => CT <= "00";
    END CASE;
  END PROCESS;

END behv;

