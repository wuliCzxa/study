LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY REGS IS
PORT (
  D  : IN  STD_LOGIC_VECTOR(15 DOWNTO 0);  -- K1, K0
  R  : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);  -- S1, S0
  SA : IN  STD_LOGIC;                      -- K4.0
  SB : IN  STD_LOGIC;                      -- K4.1
  RD : IN  STD_LOGIC;                      -- K4.2
  WR : IN  STD_LOGIC;                      -- K4.3
  RST: IN  STD_LOGIC;                      -- K4.7
  CLK: IN  STD_LOGIC                       -- CLOCK
);
END REGS;

ARCHITECTURE behv OF REGS IS

SIGNAL R0, R1, R2, R3: STD_LOGIC_VECTOR(15 DOWNTO 0);

BEGIN

  -- R0
  PROCESS(CLK, RST, WR, SA, SB)
  BEGIN
    IF RST = '1' THEN
      R0 <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (WR = '0') AND (SB = '0') AND (SA = '0') THEN
        R0 <= D;
      END IF;
    END IF;
  END PROCESS;

  -- R1
  PROCESS(CLK, RST, WR, SA, SB)
  BEGIN
    IF RST = '1' THEN
      R1 <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (WR = '0') AND (SB = '0') AND (SA = '1') THEN
        R1 <= D;
      END IF;
    END IF;
  END PROCESS;

  -- R2
  PROCESS(CLK, RST, WR, SA, SB)
  BEGIN
    IF RST = '1' THEN
      R2 <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (WR = '0') AND (SB = '1') AND (SA = '0') THEN
        R2 <= D;
      END IF;
    END IF;
  END PROCESS;

  -- R3
  PROCESS(CLK, RST, WR, SA, SB)
  BEGIN
    IF RST = '1' THEN
      R3 <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (WR = '0') AND (SB = '1') AND (SA = '1') THEN
        R3 <= D;
      END IF;
    END IF;
  END PROCESS;

  R <= R0 WHEN (SB = '0') AND (SA = '0') AND (RD = '0') ELSE
       R1 WHEN (SB = '0') AND (SA = '1') AND (RD = '0') ELSE
       R2 WHEN (SB = '1') AND (SA = '0') AND (RD = '0') ELSE
       R3 WHEN (SB = '1') AND (SA = '1') AND (RD = '0') ELSE
       (OTHERS=>'0');
    
END behv;

