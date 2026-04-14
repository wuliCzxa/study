LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY REG IS
PORT (
  D  : IN  STD_LOGIC_VECTOR(15 DOWNTO 0);  -- K1, K0
  R  : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);  -- S1, S0
  EN : IN  STD_LOGIC;                      -- K4.0
  RST: IN  STD_LOGIC;                      -- K4.7
  CLK: IN  STD_LOGIC                       -- CLOCK
);
END REG;

ARCHITECTURE behv OF REG IS

BEGIN
  PROCESS(CLK, RST, EN)
  BEGIN
    IF RST = '1' THEN
      R <= (OTHERS=>'0');
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF (EN = '0') THEN
        R <= D;
      END IF;
    END IF;
  END PROCESS;

END behv;

