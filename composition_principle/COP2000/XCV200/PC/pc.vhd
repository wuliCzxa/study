LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY pc IS
PORT (
  D  : IN  STD_LOGIC_VECTOR(15 DOWNTO 0);  -- K1, K0
  PC : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);  -- S1, S0
  C  : IN  STD_LOGIC;                      -- K4.0
  Z  : IN  STD_LOGIC;                      -- K4.1
  ELP: IN  STD_LOGIC;                      -- K4.2
  PC1: IN  STD_LOGIC;                      -- K4.3
  IR2: IN  STD_LOGIC;                      -- K4.4
  IR3: IN  STD_LOGIC;                      -- K4.5
  RST: IN  STD_LOGIC;                      -- K4.7
  CLK: IN  STD_LOGIC                       -- CLOCK
);
END PC;

ARCHITECTURE behv OF PC IS

SIGNAL RPC: STD_LOGIC_VECTOR(15 DOWNTO 0);

BEGIN

  PC <= RPC;
  
  PROCESS(CLK, RST, IR3, IR2, C, Z, ELP)
  VARIABLE LDPC: STD_LOGIC; -- ת�ƿ���
  BEGIN
    IF RST = '1' THEN
      RPC <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (ELP = '0') AND (
           ( (IR3 = '0') AND (IR2 = '0') AND (C = '1') ) OR
           ( (IR3 = '0') AND (IR2 = '1') AND (Z  = '1') ) OR
           (IR3 = '1')
         ) THEN LDPC := '0'; ELSE LDPC := '1';
      END IF;

      IF LDPC = '0' THEN
        RPC <= D;
      ELSIF (PC1 = '1') THEN  
        RPC <= RPC + 1;
      END IF;
    END IF;
  END PROCESS;

END behv;

